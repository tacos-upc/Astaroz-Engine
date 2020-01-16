#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <rapidjson/document.h>
#include <MathGeoLib.h>

class Serialization
{
public:
	Serialization();
	Serialization(const rapidjson::Value& object_value);
	Serialization(const std::string& serialized_scene_string);
	~Serialization() = default;

	Serialization(const Serialization& other);
	Serialization& operator=(const Serialization& other);

	rapidjson::Document::AllocatorType& GetAllocator() const;

	void AddInt(int value_to_add, const std::string &name);
	int GetInt(const std::string &name, int opt_value) const;

	void AddUInt(uint64_t value_to_add, const std::string& name);
	uint64_t GetUInt(const std::string& name, unsigned int opt_value) const;

	void AddFloat(float value_to_add, const std::string& name);
	float GetFloat(const std::string &name, float opt_value) const;

	void AddBool(bool value_to_add, const std::string& name);
	bool GetBool(const std::string& name, bool opt_value) const;

	void AddString(const std::string value_to_add, const std::string& name);
	void GetString(const std::string& name, std::string& return_value, const std::string& opt_value) const;

	void AddFloat3(const float3 &value_to_addvalue_to_add, const std::string& name);
	void GetFloat3(const std::string &name, float3 &return_value, const float3 &opt_value) const;

	void AddQuat(const Quat & value_to_add, const std::string& name);
	void GetQuat(const std::string& name, Quat& return_value, const Quat& opt_value) const;

	void AddColor(const float4& value_to_add, const std::string& name);
	void GetColor(const std::string& name, float4& return_value, const float4& opt_value) const;

	void AddChildConfig(Serialization& value_to_add, const std::string& name);
	void GetChildConfig(const std::string& name, Serialization& value_to_add) const;

	void AddChildrenConfig(std::vector<Serialization>& value_to_add, const std::string& name);
	void GetChildrenConfig(const std::string& name, std::vector<Serialization>& return_value) const;

	void GetSerializedString(std::string& return_string);

public:
	   rapidjson::Document config_document;
	   rapidjson::Document::AllocatorType* allocator;

};

#endif //_SERIALIZATION_H_