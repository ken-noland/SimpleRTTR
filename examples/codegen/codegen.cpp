#include <simplerttr.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <set>
#include <filesystem>

#include <mustache.hpp>

using namespace SimpleRTTR;
using namespace kainjow::mustache;


#define GENERATED_DIR "generated"
#define CACHED_TYPES_FILE "cachedtypes.txt"

#define TEMPLATE_DIR SOURCE_DIR "/templates/"

enum class TypeExportAction
{
    None,
    Add,
    Change,
    Remove
};

struct TypeExportData
{
    TypeReference type;
    std::string name;   //in situations where the type has been removed, all we will have is the type name.
    TypeExportAction action;

    std::filesystem::path sourceFilename;
    std::filesystem::path relativePath;

    std::vector<std::filesystem::path> generatedFiles;
};

struct CodeGenExportData
{
    bool hasChanged;
    bool hasAdded;
    bool hasRemoved;

    std::vector<TypeExportData> typeExportData;

    // all collected source files
    std::set<std::filesystem::path> sourceFiles;
};


struct TemplateDefinition
{
    std::string name;
    std::string source;
};


//Cached types is a class to load and store the type hashes of all the types that have been registered. It's a registry 
// of sorts which allows us to determine if a type has been added, removed, or changed.
class CachedTypes
{
public:
    CachedTypes() = default;
    ~CachedTypes() { Unload(); };

    bool Load()
    {
        std::ifstream file(CACHED_TYPES_FILE);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                //use a simple classname:hash format
                std::size_t pos = line.find(':');
                if (pos != std::string::npos)
                {
                    std::string typeName = line.substr(0, pos);
                    std::size_t typeHash = std::stoull(line.substr(pos + 1));
                    _typeHashes[typeName] = typeHash;
                }
            }

            file.close();
        }
        else
        {
            _typeHashes.clear();
            return false;
        }

        return true;
    }

    bool Save()
    {
        std::ofstream file(CACHED_TYPES_FILE);
        if (file.is_open())
        {
            //iterate through the types in Types() and save the hash
            for (Type type : Types())
            {
                file << type.FullyQualifiedName() << ":" << type.Hash() << std::endl;
            }

            file.close();
        }
        else
        {
            return false;
        }

        return true;
    }

    bool Unload()
    {
        _typeHashes.clear();
        return true;
    }

    std::size_t GetTypeHash(const std::string& typeName) const
    {
        TypeMap::const_iterator found = _typeHashes.find(typeName);
        if (found != _typeHashes.end())
        {
            return found->second;
        }
        return -1;
    }

    using TypeMap = std::unordered_map<std::string, std::size_t>;
    using TypeMapIterator = TypeMap::const_iterator;

    TypeMapIterator begin() const { return _typeHashes.begin(); }
    TypeMapIterator end() const { return _typeHashes.end(); }

protected:
    TypeMap _typeHashes;
};

std::filesystem::path GetGeneratedDir()
{
    std::filesystem::path outputDir = BINARY_DIR;
    outputDir /= GENERATED_DIR;
    return outputDir;
}


std::filesystem::path GetFilenameForTemplate(const std::string& typeName, const std::filesystem::path& relativePath, const TemplateDefinition& templateDefinition)
{
    std::filesystem::path outputFilename = GetGeneratedDir();
    outputFilename /= relativePath;
    outputFilename /= typeName + "_" + templateDefinition.name + ".cpp";
    return outputFilename;
}

// Reformat file paths to something CMake can handle
std::string GetCMakePath(const std::string& input) {
    std::string result;
    result.reserve(input.size() * 2); // Reserve space to minimize reallocations

    for (char ch : input) {
        if (ch == '\\') {
            result += "/";
        }
        else {
            result += ch;
        }
    }

    return result;
}

bool FilterTypes(const Type& type)
{
    //we don't want to generate source files for types that are not user defined.
    if (type.Meta().Has("source_filename"))
    {
        return true;
    }

    return false;
}

bool RunDiff(CodeGenExportData& exportData, const CachedTypes& cachedTypes, const std::vector<TemplateDefinition>& templateDefinitions)
{
    exportData.hasChanged = false;
    exportData.hasAdded = false;
    exportData.hasRemoved = false;

    //go through the find all types that have been added, or changed
    for (Type type : Types())
    {
        std::size_t typeHash = type.Hash();
        std::size_t cachedHash = cachedTypes.GetTypeHash(type.FullyQualifiedName());

        if (!FilterTypes(type))
        {
            continue;
        }

        std::filesystem::path sourceFilename = "";
        std::filesystem::path relativePath = "";
        std::vector<std::filesystem::path> exportFilenames;

        if (type.Meta().Has("source_filename"))
        {
            sourceFilename = type.Meta().Get("source_filename").Value().GetAs<const char*>();
            relativePath = std::filesystem::relative(sourceFilename.parent_path(), PROJECT_DIR);
            for (TemplateDefinition templateDefinition : templateDefinitions)
            {
                exportFilenames.push_back(GetFilenameForTemplate(type.Name(), relativePath, templateDefinition));
            }
            exportData.sourceFiles.insert(sourceFilename);
        }

        TypeExportData typeExportData{
            TypeReference(type),
            type.Name(),
            TypeExportAction::None,
            sourceFilename,
            relativePath,
            exportFilenames
        };

        if (cachedHash == -1)
        {
            //std::cout << "Type: \"" << type.FullyQualifiedName() << "\" has been added." << std::endl;
            typeExportData.action = TypeExportAction::Add;
            exportData.hasAdded = true;
        }
        else if (typeHash != cachedHash)
        {
            //std::cout << "Type: \"" << type.FullyQualifiedName() << "\" has changed." << std::endl;
            if (type.Meta().Has("source_filename"))
            {
                exportData.sourceFiles.insert(type.Meta().Get("source_filename").Value().GetAs<const char*>());
            }
            typeExportData.action = TypeExportAction::Change;
            exportData.hasChanged = true;
        }

        exportData.typeExportData.push_back(typeExportData);
    }

    //go through the find all types that have been removed
    for (CachedTypes::TypeMapIterator it = cachedTypes.begin(); it != cachedTypes.end(); ++it)
    {
        const Type& type = Types().GetType(it->first);
        if (type == Type::InvalidType())
        {
            //std::cout << "Type: \"" << it->first << "\" has been removed." << std::endl;
            exportData.typeExportData.push_back({ TypeReference(Type::InvalidType()), it->first, TypeExportAction::Remove, "" });
            exportData.hasRemoved = true;
        }
    }

    return true;
}

data GatherDataForCMakeLists(const CodeGenExportData& exportData)
{
    data sources{ data::type::list };
    for (const TypeExportData& exportData : exportData.typeExportData)
    {
        if (exportData.action == TypeExportAction::None || exportData.action == TypeExportAction::Add || exportData.action == TypeExportAction::Change)
        {
            for (const std::filesystem::path& generatedFile : exportData.generatedFiles)
            {
                sources << GetCMakePath(generatedFile.string());
            }
        }
    }

    data sourceLocations{ data::type::list };
    sourceLocations << GetCMakePath(SOURCE_DIR);

    data cmakeListsData;
    cmakeListsData.set("sources", sources);
    cmakeListsData.set("source_dir", SOURCE_DIR);
    cmakeListsData.set("source_locations", sourceLocations);

    return cmakeListsData;
}

void ExportCMakeLists(const CodeGenExportData& exportData)
{
    //open the CMakeLists.txt template file
    std::ifstream templateFile(TEMPLATE_DIR "CMakeLists.txt.in");
    if (!templateFile.is_open())
    {
        std::cerr << "Failed to open file: " << TEMPLATE_DIR "CMakeLists.txt.in" << std::endl;
        exit(1);
    }

    //read the template file into a string
    std::string templateString((std::istreambuf_iterator<char>(templateFile)), std::istreambuf_iterator<char>());

    //close the template file
    templateFile.close();

    //now move onto the output file
    std::filesystem::path cmakeListsPath = GetGeneratedDir();
    std::filesystem::create_directories(cmakeListsPath); //ensure the path exists

    //gather data for the template
    data cmakeListsData = GatherDataForCMakeLists(exportData);

    //open the output file
    std::filesystem::path cmakeListsFilePath = cmakeListsPath / "CMakeLists.txt";
    std::ofstream cmakeListsFile(cmakeListsFilePath);
    if (!cmakeListsFile.is_open())
    {
        std::cerr << "Failed to open file: " << cmakeListsFilePath << std::endl;
        exit(1);
    }

    //render out the template
    mustache tmpl(templateString);
    tmpl.render(cmakeListsData, [&cmakeListsFile](const std::string& str) {
        cmakeListsFile << str;
        });

    cmakeListsFile.close();
}


void ProcessSourceFile(std::ostream& output, const std::filesystem::path& sourceFilename)
{
    //read the source file line by line
    std::ifstream file(sourceFilename);
    if (file.is_open())
    {
        bool multiline = false;
        std::string line;
        while (std::getline(file, line))
        {
            //check for the hash symbol which indicates a preprocessor directive
            if (line[0] == '#' || multiline)
            {
                //check for include directive that use quotes and use sourceFilename.parent_path() to resolve 
                // the path
                if (line.find("#include") != std::string::npos && line.find("<") == std::string::npos)
                {
                    std::size_t pos = line.find("\"");
                    if (pos != std::string::npos)
                    {
                        std::size_t endPos = line.find("\"", pos + 1);
                        if (endPos != std::string::npos)
                        {
                            std::string includePath = line.substr(pos + 1, endPos - pos - 1);
                            std::filesystem::path includePathFull = sourceFilename.parent_path() / includePath;
                            output << "#include \"" << includePathFull.string() << "\"" << std::endl;
                        }
                    }
                }
                else
                {
                    //we need to process all preprocessor directives so that if an include is between a ifdef/endif block, we can still get 
                    // the correct include directive.
                    output << line << std::endl;
                }


                //check for trailing backslash which indicates a multiline preprocessor directive
                if (line[line.size() - 1] == '\\')
                {
                    multiline = true;
                }
                else
                {
                    multiline = false;
                }
            }
        }

        file.close();
    }
    else
    {
        std::cerr << "Failed to open file: " << sourceFilename << std::endl;
        exit(1);
    }
}

void ProcessSourceFiles(std::unordered_map<std::filesystem::path, std::string>& sourceFileHeaderData, const CodeGenExportData& exportData)
{
    for (const std::filesystem::path& sourceFilename : exportData.sourceFiles)
    {
        std::stringstream ss;
        ProcessSourceFile(ss, sourceFilename);
        sourceFileHeaderData[sourceFilename] = ss.str();
    }
}

bool GatherPropertyData(data& propertyData, const Property& property)
{
    propertyData.set("name", property.Name());
    propertyData.set("offset", std::to_string(property.Offset()));

    data typeData{ data::type::object };
    Type type = property.Type();
    typeData.set("name", type.Name());
    typeData.set("fullyQualifiedName", type.FullyQualifiedName());

    propertyData.set("type", typeData);



    return true;
}

bool GatherTypeData(data& typeData, const TypeExportData& typeExportData)
{
    const Type& type = typeExportData.type.Type();

    typeData.set("name", type.Name());
    typeData.set("fullyQualifiedName", type.FullyQualifiedName());
    typeData.set("sourceFilename", typeExportData.sourceFilename.string());
    typeData.set("relativePath", typeExportData.relativePath.string());

    //TODO: deal with metadata
    //data meta{ data::type::list };
    //for (const Meta& meta : type.Meta())
    //{
    //    if(meta.Key().Type() == )
    //    data metaItem;
    //    metaItem.set("key", key);
    //    metaItem.set("value", value.Value().GetAs<const char*>());
    //    meta << metaItem;
    //}

    data namespaces{ data::type::list };
    for (const std::string& ns : type.Namespaces())
    {
        namespaces << ns;
    }
    typeData.set("namespaces", namespaces);

    data templateParams{ data::type::list };
    for (const TypeReference& templateParam : type.TemplateParams())
    {
        Type templateType = templateParam.Type();

        data templateParamData;
        templateParamData.set("name", templateType.Name());
        templateParamData.set("fullyQualifiedName", templateType.FullyQualifiedName());
        templateParamData.set("hash", templateType.Hash());

        templateParamData << templateParamData;
    }
    typeData.set("templateParams", templateParams);

    data properties{ data::type::list };
    for(const Property& property : type.Properties())
    { 
        data propertyData;
        GatherPropertyData(propertyData, property);

        propertyData.set("root", typeData);
        properties << propertyData;
    }
    typeData.set("properties", properties);

    return true;
}

int main(int arc, char** argv)
{
    std::vector<TemplateDefinition> templates;

//    templates.push_back(TemplateDefinition{ "Json", TEMPLATE_DIR "JsonTemplate.cpp.in" });
    templates.push_back(TemplateDefinition{ "Lua", TEMPLATE_DIR "LuaTemplate.cpp.in" });

    bool regenerateAll = false;

    CachedTypes cachedTypes;
    if (!cachedTypes.Load())
    {
        //first time running?
        regenerateAll = true;
    }

    CodeGenExportData exportData;
    RunDiff(exportData, cachedTypes, templates);

    //early out for no changes
    if (!exportData.hasAdded && !exportData.hasChanged && !exportData.hasRemoved)
    {
        return 0;
    }

    //only update the CMakeLists.txt file if we have added or removed a source file
    if (regenerateAll || exportData.hasAdded || exportData.hasRemoved)
    {
        ExportCMakeLists(exportData);
    }

    // parse out the source files to get the includes from the preprocessor directives
    std::unordered_map<std::filesystem::path, std::string> sourceFileHeaderData;
    ProcessSourceFiles(sourceFileHeaderData, exportData);

    //we have a list of types, so now let's go about generating the source files for them.
    for (const TypeExportData& typeExportData : exportData.typeExportData)
    {
        //only run codegen on types that have been added or changed, or if the template has been modified
        //  TODO: detect when template has been modified
        if (typeExportData.action == TypeExportAction::Add || typeExportData.action == TypeExportAction::Change)
        {
            const Type& type = typeExportData.type.Type();

            //construct the data from the type information
            data typeData;
            GatherTypeData(typeData, typeExportData);

            //TODO: lazy construct the header data
            typeData.set("header", sourceFileHeaderData[typeExportData.sourceFilename.string()]);

            int templateIndex = 0;
            for (TemplateDefinition& templateDef : templates)
            {
                std::ifstream file(templateDef.source);
                if (file.is_open())
                {
                    //TODO: should only need to open the template file once
                    std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                    file.close();

                    mustache tmpl(source);
                    tmpl.set_custom_escape([](const std::string& str) {
                        return str;
                        });

                    std::filesystem::path outputFilename = typeExportData.generatedFiles[templateIndex];
                    
                    //ensure the path to the file exists
                    std::filesystem::create_directories(outputFilename.parent_path());

                    std::ofstream outputFile(outputFilename);
                    if (!outputFile.is_open())
                    {
                        std::cerr << "Failed to open file: " << outputFilename << std::endl;
                        exit(1);
                    }

                    tmpl.render(typeData, [&outputFile](const std::string& str) {
                        outputFile << str;
                        });

                    outputFile.close();
                }
                templateIndex++;
            }
        }
    }

    //when adding or modifying a type, we need to create a source file for it or overwrite the existing one.



    //we only need to update the makefile if we have added or removed a type, otherwise we can leave it alone.

    return 0;
}
