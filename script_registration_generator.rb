def grabFiles directory
  puts "Directory: #{directory}"
  files = Dir.entries(directory)
  files.collect! { |f| directory + f }
  delete_files = []
  files.each do |file|
    if file =~ /.*\.\.?\Z/
      delete_files.push file
    elsif File.directory?(file)
      delete_files.push file
      files << grabFiles(file + "/")
      files.flatten!
    end
  end
  
  files.delete_if { |f| delete_files.count(f) > 0 }

  return files
end

def cullNonScriptableFiles files
  h_files = []

  files.each do |file|
    if file =~ /\A.*\.hp{2}?\Z/
      h_files << file 
    end
  end

  scriptable_files = []
  scriptable_filenames = []
  h_files.each do |filename|
    file = File.readlines(filename).join("")

    if file =~ /\s*\/\/=\s*SCRIPTABLE\s*$/
      scriptable_filenames << filename
      scriptable_files << file
    end
  end

  return scriptable_files, scriptable_filenames
end

def extractNamespace file 
  namespace_regex = /\s*namespace\s+(\w+)\s*{\s*/
  matches = file.scan(namespace_regex)

  if matches.size > 0
    return matches.join("::")
  else
    return nil
  end
end

def extractClassNameAndBody file
  class_regex = /^(?!enum)\s*class\s+([a-zA-Z]+)[^;]*$.*\/\/=\s*BEGIN\s*SCRIPTABLE(.*)\/\/=\s*END\s*SCRIPTABLE.*?}/m
  matches = class_regex.match(file)
  if matches != nil
    return matches.captures[0], matches.captures[1]
  else
    return nil
  end
end

def extractBaseClasses file
  base_class_regex = /\/\/=\s*SCRIPTABLE\s*BASES(.*)$/
  
  matches = base_class_regex.match(file)

  if matches != nil
    base_classes = matches.captures[0].split(" ").collect {|base| base.strip }
    return base_classes
  else
    return nil
  end
end

def extractMethods text, full_text
  methods = {}
  text = text.split "\n"

  function_regex = /([^:]*?)?\s+(\w*?)\((.*?),?\)\s*.*$/

  template_types = []
  text.each do |line|
    matches = function_regex.match(line)
    if !line.include?("= delete") && matches != nil
      type = matches.captures[0].strip
      name = matches.captures[1].strip
      args = matches.captures[2].strip
      overloaded = full_text.scan(/\s#{Regexp.quote(name)}\(/).count > 1
      static = line.include?("static ")


      arg_list = matches.captures[2].split(",").map { |a| a.split(/\s=/)[0] }
      if(methods[name] == nil)
        methods[name] = [[type, arg_list, overloaded, static]]
      else
        methods[name] << [type, arg_list, overloaded, static]
      end
    end
  end
  return methods
end

def extractEnums file_text
  enums = {}

  scriptable_enum_regex = /\s*\/\/=\s*SCRIPTABLE\s*ENUM\s*enum\s*[clas]*\s+(\w+)(\W*|\s*:\s*[a-zA-Z ]+){(.*?)};/m

  class_regex = /^(?!enum)\s*class\s+([a-zA-Z]+).*/

  matches = file_text.scan(scriptable_enum_regex)

  class_matches = class_regex.match(file_text)

  if matches != []
    matches.each do |match|
      name = match[0]

      if class_matches != nil && file_text.index(class_matches[0]) < file_text.index(/\/\/=\s*SCRIPTABLE\s*ENUM\s*/)
        name = class_matches.captures[0] + "::" + name
      end

      args = match[2].split(",").collect {|val| val.strip }

      enums[name] = args
    end
  end

  return enums
end

def generateGetModule class_name, metadata, base_classes
   constructors = []
   methods = []

   metadata.each do |func_name, func_meta|

    func_meta.each do |overload|
      args = overload[1].join(", ")
      #NO RETURN TYPE
      if overload[0].empty?
        constructors.push "chaiscript::constructor<#{class_name}(#{args})>(),"
      #RETURN TYPE
      else
        overload[0].slice!("static ")
        if overload[2] == false
          methods.push "chaiscript::fun(&#{class_name}::#{func_name}), \"#{overload[3]?class_name + "_" : ""}#{func_name}\""
        else
          methods.push "chaiscript::fun([](#{class_name}& c#{args.length > 0 ? ", " + args : ""}) { c.#{func_name}(#{args.length > 0 ? (args.split(", ").map { |a| a.split(" ")[-1]}).join(", ") : ""}); }), \"#{func_name}\""
          #methods.push "chaiscript::fun<#{overload[0]}, #{class_name}#{args.length > 0 ? ", " + args : ""}>(&#{class_name}::#{func_name}), \"#{func_name}\""
        end
      end
    end
   end

  get_module = %?chaiscript::ModulePtr get#{class_name}Module() {
    chaiscript::ModulePtr module = std::make_shared<chaiscript::Module>();

    chaiscript::utility::add_class<#{class_name}>(*module, std::string("#{class_name}"),
    ?
  
  get_module += "{\n"
  constructors.each do |c|
    get_module += c + "\n"
  end

  get_module += "},\n"

  get_module += "{\n"
  methods.each do |m|
    get_module += "{" + m + "},\n"
  end

  get_module.chomp! ","


  get_module += "});\n"

  if base_classes != nil
    base_classes.each do |b|
      get_module += "  module->add(chaiscript::base_class<#{b}, #{class_name}>());\n"
    end
  end
  get_module += "return module;\n"
  get_module += "}\n"
    
  return get_module, "get#{class_name}Module()"
end

def generateEnumModule enum_name, enum_contents
  get_module = %?chaiscript::ModulePtr get#{enum_name.split("::").join("")}Module() {
    chaiscript::ModulePtr module = std::make_shared<chaiscript::Module>();
    std::vector<std::pair<unsigned int, std::string>> vec = {?

  enum_contents.each do |c|
    get_module += "{ #{enum_name.split('::').size > 1 ? enum_name.split("::")[0] + "::" : ""}#{c}, \"#{enum_name.split("::").join("_")}_#{c}\" },\n"
  end

  get_module += "};\n"

  get_module += "chaiscript::utility::add_class<#{enum_name}>(*module, std::string(\"#{enum_name.split("::").join("_")}\"),\n"
  get_module += "vec);\n"

  get_module += "return module;\n"
  get_module += "}\n"

  return get_module, "get#{enum_name.split("::").join("")}Module()"
end

def getAllBases klass, names_to_bases
  bases = names_to_bases[klass]
  
  if bases != nil
    bases.each do |base|
      next_bases = getAllBases(base, names_to_bases)
      if next_bases != nil
        bases += getAllBases(base, names_to_bases)
      end
    end
  end

  return bases
end


files = grabFiles("./src/")

culled_files, culled_filenames = cullNonScriptableFiles(files)

class_names_to_bodies = {}
class_names_to_file_text = {}
class_names_to_filenames =  {}
class_names_to_base_classes = {}
namespaces = []
enums = {}
enum_filenames = []

culled_files.zip(culled_filenames).each do |file, filename|
  class_name, body = extractClassNameAndBody(file)
  namespace = extractNamespace(file)
  bases = extractBaseClasses(file)

  ems = extractEnums(file)
  if(ems != {})
    enums.merge! ems
    enum_filenames << filename
  end
  
  if(namespace != nil && namespaces.count(namespace) == 0)
    namespaces << namespace
  end

  if(class_name != nil)
    class_names_to_bodies[class_name] = body
    class_names_to_file_text[class_name] = file
    class_names_to_filenames[class_name] = filename
    class_names_to_base_classes[class_name] = bases
  end
end

#cascade up the bases
ctb = class_names_to_base_classes.clone

ctb.each do |klass, bases|
  if bases != nil 
    bases.each do |base|
      if ctb[base] != nil
        class_names_to_base_classes[klass] += getAllBases(base, ctb)
        class_names_to_base_classes[klass].uniq!
      end
    end
  end
end

classes = {}

class_names_to_bodies.each_pair do |key, value| 
  classes[key] = extractMethods(value, class_names_to_file_text[key])
end

func_def_file = %?#ifndef GENERATED_REGISTRATIONS_H
#define GENERATED_REGISTRATIONS_H
//THIS FILE WAS GENERATED BY script_registration_generator.rb
//DO NOT FUCK WITH IT AS IT WILL BE OVERWRITTEN ON COMPILE
?

filenames = class_names_to_filenames.values | enum_filenames

func_def_file += "#include <vector>\n#include <map>\n"
filenames.each do |fn|
  fn.slice! "./src/"
  func_def_file += "#include \"#{fn}\"\n"
end


func_def_file += "namespace generated {\n"

namespaces.each do  |n|
  func_def_file += "  using namespace #{n};\n"
end

func_calls = []

enums.each_pair do |name, vals|
  mod, func_name = generateEnumModule(name, vals)

  func_def_file += "\n#{mod}\n"
  func_calls << func_name
end

classes.each_pair do |name, meta|
  mod, func_name = generateGetModule(name, meta, class_names_to_base_classes[name])

  func_def_file += "\n#{mod}\n"
  func_calls << func_name
end


func_def_file += "void registerModules(std::shared_ptr<chaiscript::ChaiScript> chai) {\n"

func_calls.each do |f|
  func_def_file += "chai->add(#{f});\n"
end

func_def_file += "}\n"
func_def_file += "}\n"
func_def_file += "#endif"

if(!Dir.exists? "./src/generated")
  Dir.mkdir("./src/generated")
end

File.open("./src/generated/generated_registrations.h", "w") { |f| f.write(func_def_file) }
