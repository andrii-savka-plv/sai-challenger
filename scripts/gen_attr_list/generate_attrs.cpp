#include <iostream> 
#include <sai.h>
#include <saimetadata.h>
#include <map>
#include <vector>


using sai_attr_info_t = std::map<std::string, std::string>; 


sai_status_t print_json(std::string &obj_type,
		        std::string &obj_desc,
		        std::vector<sai_attr_info_t> &attr_info)
{
    /* TODO: generate JSON for SAI obj */
    /* Found the following lib for generation:
     * https://github.com/json-c/json-c
     */

    return SAI_STATUS_NOT_IMPLEMENTED;
}


std::string attr_flags_get(const sai_attr_metadata_t *meta)
{
    /* Translate SAI flags to string */
    std::vector<std::string> flags;
    if (meta->ismandatoryoncreate)
	flags.push_back("MANDATORY_ON_CREATE");
    if (meta->iscreateonly)
	flags.push_back("CREATE_ONLY");
    if (meta->iscreateandset)
	flags.push_back("CREATE_AND_SET");
    if (meta->isreadonly)
	flags.push_back("READ_ONLY");
    if (meta->iskey)
        flags.push_back("KEY");

    /* prepare required JSON format */
    if (flags.empty())
        return std::string();

    std::string flags_dict = { "[" };;
    for (const auto& str : flags)
    {
	flags_dict += (flags_dict.size() > 1) ? (", " + str) : str;
    }
    flags_dict.push_back(']');
    return flags_dict;
}

sai_status_t generate_attr_list(sai_object_type_t obj_type)
{
    if (obj_type >= sai_metadata_attr_by_object_type_count)
        return SAI_STATUS_FAILURE;

    /* struct sai_attr_metadata_t metadata for attr is described in sai/meta/saimetadatatypes.h:752 in SAI repo */
    const sai_attr_metadata_t * const* const attrs = sai_metadata_attr_by_object_type[obj_type];

    /* Prepare map with required attr data */
    std::vector<sai_attr_info_t> obj_info;

    int idx = 0;
    while (attrs[idx] != NULL)
    {
        sai_attr_info_t attr_info;
        attr_info.emplace("name", attrs[idx]->attridname);
	attr_info.emplace("description", attrs[idx]->brief);

	std::string flags = attr_flags_get(attrs[idx]);
	if (!flags.empty())
            attr_info.emplace("flags", flags);

	const char* ptr = sai_metadata_get_attr_value_type_name(attrs[idx]->attrvaluetype);
	attr_info.emplace("type", (ptr != nullptr) ? ptr : "unknown");

        obj_info.push_back(attr_info);
        idx++;
    }

    std::string obj_type_str = { sai_metadata_get_object_type_name(obj_type) };
    std::string obj_desc = obj_type_str;
    /* !There is NO obj description in SAI metadata */
    obj_desc.replace(obj_desc.begin(), obj_desc.end(), '_', ' ');
   
    /* Print SAI obj info to JSON  */
    return print_json(obj_type_str, obj_desc, obj_info);
}

int main()
{

    for (int obj_type = 0; obj_type < SAI_OBJECT_TYPE_MAX; ++obj_type)
    {
        generate_attr_list(static_cast<sai_object_type_t>(obj_type));
    }

    return 0;
}
