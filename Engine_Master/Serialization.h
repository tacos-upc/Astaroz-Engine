#ifndef _SERIALIZATION_H_
#define _SERIALIZATION_H_

#include <rapidjson/document.h>
#include <MathGeoLib.h>

class Serialization
{
public:
	Serialization();
	Serialization(const std::string& serializedScene);
	Serialization(const rapidjson::Value& value);
	Serialization(const Serialization& serial);
	
	~Serialization() = default;

	Serialization& operator = (const Serialization& serial);

	//public methods
	rapidjson::Document::AllocatorType& GetAllocatorType() const;
	void GetSerializedScene(std::string& serializedScene);

	void AddInt(int value, const std::string& name);
	int GetInt(const std::string& name, int value) const;
	void AddBool(bool value, const std::string& name);
	bool GetBool(const std::string& name, bool value) const;
	void AddUInt(uint64_t value, const std::string& name);
	uint64_t GetUInt(const std::string& name, unsigned int value) const;
	void AddFloat(float value, const std::string& name);
	float GetFloat(const std::string& name, float value) const;
	void AddFloat3(const float3& value, const std::string& name);
	void GetFloat3(const std::string& name, float3& returnValue, const float3& optValue) const;
	void AddString(const std::string value, const std::string& name);
	void GetString(const std::string& name, std::string& returnValue, const std::string& optValue) const;
	void AddQuat(const Quat& value, const std::string& name);
	void GetQuat(const std::string& name, Quat& returnValue, const Quat& optValue) const;
	void AddColor(const float4& value, const std::string& name);
	void GetColor(const std::string& name, float4& returnValue, const float4& optValue) const;
	void AddChildConfig(Serialization& value, const std::string& name);
	void GetChildConfig(const std::string& name, Serialization& value) const;
	void AddChildrenSerial(std::vector<Serialization>& value, const std::string& name);
	void GetChildrenSerial(const std::string& name, std::vector<Serialization>& value) const;

	//public variables
	rapidjson::Document document;
	rapidjson::Document::AllocatorType* allocatorType;
};

#endif //_SERIALIZATION_H_