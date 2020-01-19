#include "Serialization.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

Serialization::Serialization()
{
	document.SetObject();
	allocatorType = &document.GetAllocator();
}

Serialization::Serialization(const Serialization& serial)
{
	document.CopyFrom(serial.document, document.GetAllocator());
	allocatorType = &document.GetAllocator();
}

Serialization::Serialization(const std::string& serializedScene)
{
	document.Parse(serializedScene.c_str());
	allocatorType = &document.GetAllocator();
}

Serialization::Serialization(const rapidjson::Value& value)
{
	document.CopyFrom(value, document.GetAllocator());
}

Serialization& Serialization::operator = (const Serialization& serial)
{
	document.CopyFrom(serial.document, document.GetAllocator());
	allocatorType = &document.GetAllocator();
	return *this;
}

rapidjson::Document::AllocatorType& Serialization::GetAllocatorType() const
{
	return *allocatorType;
}

void Serialization::SaveInt(const std::string& name, int value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

int Serialization::LoadInt(const std::string& name, int defaultValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		return defaultValue;
	}
	else
	{
		const rapidjson::Value& intValue = document[name.c_str()];
		return intValue.GetInt();
	}
}

void Serialization::SaveBool(const std::string& name, bool value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

bool Serialization::LoadBool(const std::string& name, bool defaultValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		return defaultValue;
	}
	else
	{
		const rapidjson::Value& boolValue = document[name.c_str()];
		return boolValue.GetBool();
	}
}

void Serialization::SaveFloat(const std::string & name, const float value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

float Serialization::LoadFloat(const std::string & name, const float defaultValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		return defaultValue;
	}
	else
	{
		const rapidjson::Value& floatValue = document[name.c_str()];
		return floatValue.GetFloat();
	}
}

void Serialization::SaveFloat3(const std::string& name, const float3& value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value array_value(rapidjson::kArrayType);
	array_value.PushBack(value.x, *allocatorType);
	array_value.PushBack(value.y, *allocatorType);
	array_value.PushBack(value.z, *allocatorType);

	document.AddMember(memberName, array_value, *allocatorType);
}

float3 Serialization::LoadFloat3(const std::string& name, const float3& defaultValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		return defaultValue;
	}
	else
	{
		const rapidjson::Value& floatValue = document[name.c_str()];
		return float3(
			floatValue[0].GetFloat(),
			floatValue[1].GetFloat(),
			floatValue[2].GetFloat()
		);
	}
}

void Serialization::SaveStringSerial(const std::string& name, const std::string& value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value stringValue(value.c_str(), value.size(), *allocatorType);
	document.AddMember(memberName, stringValue, *allocatorType);
}

std::string Serialization::LoadStringSerial(const std::string& name, const std::string& defaultValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		return defaultValue;
	}
	else
	{
		const rapidjson::Value& stringValue = document[name.c_str()];
		return std::string(stringValue.GetString(), stringValue.GetStringLength());
	}
}

std::string Serialization::LoadSerializedScene()
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	document.Accept(writer);
	return buffer.GetString();
}

void Serialization::SaveQuat(const std::string& name, const Quat& value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value quat_value(rapidjson::kArrayType);
	quat_value.PushBack(value.x, *allocatorType);
	quat_value.PushBack(value.y, *allocatorType);
	quat_value.PushBack(value.z, *allocatorType);
	quat_value.PushBack(value.w, *allocatorType);

	document.AddMember(memberName, quat_value, *allocatorType);
}

Quat Serialization::LoadQuat(const std::string& name, const Quat& defaultValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		return defaultValue;
	}
	else
	{
		const rapidjson::Value& value = document[name.c_str()];
		return Quat(
			value[0].GetFloat(),
			value[1].GetFloat(),
			value[2].GetFloat(),
			value[3].GetFloat()
		);
	}
}

void Serialization::SaveChildrenSerial(const std::string& name, std::vector<Serialization>& value)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value childrenSerial(rapidjson::kArrayType);
	for (unsigned int i = 0; i < value.size(); i++)
	{
		rapidjson::Value temp(rapidjson::kObjectType);
		temp.CopyFrom(value[i].document.Move(), *allocatorType);
		childrenSerial.PushBack(temp.Move(), *allocatorType);
	}
	document.AddMember(memberName, childrenSerial, *allocatorType);
}

std::vector<Serialization> Serialization::LoadChildrenSerial(const std::string& name) const
{
	const rapidjson::Value& childrenSerial = document[name.c_str()];
	std::vector<Serialization> serialList = std::vector<Serialization>();
	for (unsigned int i = 0; i < childrenSerial.Size(); i++)
	{
		const rapidjson::Value& temp = childrenSerial[i];
		Serialization tmp_config(temp);
		serialList.push_back(tmp_config);
	}
	return serialList;
}