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

	void AddInt(const std::string& name, int value);
	int GetInt(const std::string& name, int value) const;
	void AddBool(const std::string& name, bool value);
	bool GetBool(const std::string& name, bool value) const;
	void AddFloat(const std::string& name, float value);
	float GetFloat(const std::string& name, float value) const;
	void AddFloat3(const std::string& name, const float3& value);
	void GetFloat3(const std::string& name, float3& returnValue, const float3& optValue) const;
	void AddString(const std::string& name, const std::string value);
	std::string GetString(const std::string& name, const std::string& optValue) const;
	void AddQuat(const std::string& name, const Quat& value);
	void GetQuat(const std::string& name, Quat& returnValue, const Quat& optValue) const;
	void AddColor(const float4& value, const std::string& name);
	void GetColor(const std::string& name, float4& returnValue, const float4& optValue) const;
	void AddChildrenSerial(const std::string& name, std::vector<Serialization>& value);
	void GetChildrenSerial(const std::string& name, std::vector<Serialization>& value) const;

	//public variables
	rapidjson::Document document;
	rapidjson::Document::AllocatorType* allocatorType;
};

#endif //_SERIALIZATION_H_