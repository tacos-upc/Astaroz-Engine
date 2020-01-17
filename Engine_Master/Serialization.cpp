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

void Serialization::AddInt(int value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

int Serialization::GetInt(const std::string& name, int value) const
{
	if (!document.HasMember(name.c_str()))
	{
		return value;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		return currentValue.GetInt();
	}
}

void Serialization::AddBool(bool value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

bool Serialization::GetBool(const std::string& name, bool value) const
{
	if (!document.HasMember(name.c_str()))
	{
		return value;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		return currentValue.GetBool();
	}
}

void Serialization::AddUInt(uint64_t value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

uint64_t Serialization::GetUInt(const std::string& name, unsigned int value) const
{
	if (!document.HasMember(name.c_str()))
	{
		return value;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		return currentValue.GetUint64();
	}
}

void Serialization::AddFloat(float value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	document.AddMember(memberName, value, *allocatorType);
}

float Serialization::GetFloat(const std::string& name, float value) const
{
	if (!document.HasMember(name.c_str()))
	{
		return value;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		return currentValue.GetFloat();
	}
}

void Serialization::AddFloat3(const float3& value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value array_value(rapidjson::kArrayType);
	array_value.PushBack(value.x, *allocatorType);
	array_value.PushBack(value.y, *allocatorType);
	array_value.PushBack(value.z, *allocatorType);

	document.AddMember(memberName, array_value, *allocatorType);
}

void Serialization::GetFloat3(const std::string& name, float3& returnValue, const float3& optValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		returnValue = optValue;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		returnValue = float3(
			currentValue[0].GetFloat(),
			currentValue[1].GetFloat(),
			currentValue[2].GetFloat()
		);
	}
}

void Serialization::AddString(const std::string value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value stringValue(value.c_str(), value.size(), *allocatorType);
	document.AddMember(memberName, stringValue, *allocatorType);
}

void Serialization::GetString(const std::string& name, std::string& returnValue, const std::string& optValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		returnValue = optValue;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		returnValue = std::string(currentValue.GetString(), currentValue.GetStringLength());
	}
}

void Serialization::GetSerializedScene(std::string& serializedScene)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	document.Accept(writer);
	serializedScene = buffer.GetString();
}

void Serialization::AddColor(const float4& value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value color_value(rapidjson::kArrayType);
	color_value.PushBack(value.x, *allocatorType);
	color_value.PushBack(value.y, *allocatorType);
	color_value.PushBack(value.z, *allocatorType);
	color_value.PushBack(value.w, *allocatorType);

	document.AddMember(memberName, color_value, *allocatorType);
}

void Serialization::GetColor(const std::string& name, float4& returnValue, const float4& optValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		returnValue = optValue;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		returnValue = float4(
			currentValue[0].GetFloat(),
			currentValue[1].GetFloat(),
			currentValue[2].GetFloat(),
			currentValue[3].GetFloat()
		);
	}
}

void Serialization::AddQuat(const Quat& value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value quat_value(rapidjson::kArrayType);
	quat_value.PushBack(value.x, *allocatorType);
	quat_value.PushBack(value.y, *allocatorType);
	quat_value.PushBack(value.z, *allocatorType);
	quat_value.PushBack(value.w, *allocatorType);

	document.AddMember(memberName, quat_value, *allocatorType);
}

void Serialization::GetQuat(const std::string& name, Quat& returnValue, const Quat& optValue) const
{
	if (!document.HasMember(name.c_str()))
	{
		returnValue = optValue;
	}
	else
	{
		const rapidjson::Value& currentValue = document[name.c_str()];
		returnValue = Quat(
			currentValue[0].GetFloat(),
			currentValue[1].GetFloat(),
			currentValue[2].GetFloat(),
			currentValue[3].GetFloat()
		);
	}
}

void Serialization::AddChildConfig(Serialization& value, const std::string& name)
{
	rapidjson::Value memberName(name.c_str(), *allocatorType);
	rapidjson::Value temp(rapidjson::kObjectType);
	temp.CopyFrom(value.document, *allocatorType);
	document.AddMember(memberName, temp, *allocatorType);
}

void Serialization::GetChildConfig(const std::string& name, Serialization& value) const
{
	//assert(document.HasMember(name.c_str()));

	const rapidjson::Value& temp = document[name.c_str()];
	value = Serialization(temp);
}

void Serialization::AddChildrenSerial(std::vector<Serialization>& value, const std::string& name)
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

void Serialization::GetChildrenSerial(const std::string& name, std::vector<Serialization>& returnValue) const
{
	//assert(document.HasMember(name.c_str()));

	const rapidjson::Value& childrenSerial = document[name.c_str()];
	returnValue = std::vector<Serialization>();
	for (unsigned int i = 0; i < childrenSerial.Size(); i++)
	{
		const rapidjson::Value& temp = childrenSerial[i];
		Serialization tmp_config(temp);
		returnValue.push_back(tmp_config);
	}
}