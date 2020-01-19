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
	std::string LoadSerializedScene();

	void SaveInt(const std::string& name, int value);
	int LoadInt(const std::string& name, int defaultValue) const;
	void SaveBool(const std::string& name, bool value);
	bool LoadBool(const std::string& name, bool defaultValue) const;
	void SaveFloat3(const std::string& name, const float3& value);
	float3 LoadFloat3(const std::string& name, const float3& defaultValue) const;
	void SaveStringSerial(const std::string& name, const std::string& value);
	std::string LoadStringSerial(const std::string& name, const std::string& defaultValue) const;
	void SaveQuat(const std::string& name, const Quat& value);
	Quat LoadQuat(const std::string& name, const Quat& defaultValue) const;
	void SaveChildrenSerial(const std::string& name, std::vector<Serialization>& value);
	std::vector<Serialization> LoadChildrenSerial(const std::string& name) const;

	//public variables
	rapidjson::Document document;
	rapidjson::Document::AllocatorType* allocatorType;
};

#endif //_SERIALIZATION_H_