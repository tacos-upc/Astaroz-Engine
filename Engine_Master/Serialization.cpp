#include "Serialization.h"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

Serialization::Serialization()
{
	config_document.SetObject();
	allocator = &config_document.GetAllocator();
}

Serialization::Serialization(const Serialization& other)
{
	config_document.CopyFrom(other.config_document, config_document.GetAllocator());
	allocator = &config_document.GetAllocator();
}

Serialization::Serialization(const std::string& serialized_scene_string)
{
	config_document.Parse(serialized_scene_string.c_str());
	allocator = &config_document.GetAllocator();
}

Serialization::Serialization(const rapidjson::Value& object_value)
{
	config_document.CopyFrom(object_value, config_document.GetAllocator());
}

Serialization& Serialization::operator=(const Serialization& other)
{
	config_document.CopyFrom(other.config_document, config_document.GetAllocator());
	allocator = &config_document.GetAllocator();
	return *this;
}

rapidjson::Document::AllocatorType& Serialization::GetAllocator() const
{
	return *allocator;
}

void Serialization::AddInt(int value_to_add, const std::string &name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	config_document.AddMember(member_name, value_to_add, *allocator);
}

int Serialization::GetInt(const std::string& name, int opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return current_value.GetInt();
	}
}

void Serialization::AddBool(bool value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	config_document.AddMember(member_name, value_to_add, *allocator);
}

bool Serialization::GetBool(const std::string& name, bool opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return current_value.GetBool();
	}
}

void Serialization::AddUInt(uint64_t value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	config_document.AddMember(member_name, value_to_add, *allocator);
}

uint64_t Serialization::GetUInt(const std::string& name, unsigned int opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return current_value.GetUint64();
	}
}

void Serialization::AddFloat(float value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	config_document.AddMember(member_name, value_to_add, *allocator);
}

float Serialization::GetFloat(const std::string& name, float opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return current_value.GetFloat();
	}
}

void Serialization::AddFloat3(const float3& value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	rapidjson::Value array_value(rapidjson::kArrayType);
	array_value.PushBack(value_to_add.x, *allocator);
	array_value.PushBack(value_to_add.y, *allocator);
	array_value.PushBack(value_to_add.z, *allocator);

	config_document.AddMember(member_name, array_value, *allocator);
}

void Serialization::GetFloat3(const std::string& name, float3 &return_value, const float3 &opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return_value = opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return_value = float3(
			current_value[0].GetFloat(),
			current_value[1].GetFloat(),
			current_value[2].GetFloat()
		);
	}
}

void Serialization::AddString(const std::string value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	rapidjson::Value string_value(value_to_add.c_str(), value_to_add.size(), *allocator);
	config_document.AddMember(member_name, string_value, *allocator);
}

void Serialization::GetString(const std::string& name, std::string& return_value, const std::string& opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return_value = opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return_value = std::string(current_value.GetString(), current_value.GetStringLength());
	}
}

void Serialization::GetSerializedString(std::string& return_string)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	config_document.Accept(writer);
	return_string = buffer.GetString();
}

void Serialization::AddColor(const float4& value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	rapidjson::Value color_value(rapidjson::kArrayType);
	color_value.PushBack(value_to_add.x, *allocator);
	color_value.PushBack(value_to_add.y, *allocator);
	color_value.PushBack(value_to_add.z, *allocator);
	color_value.PushBack(value_to_add.w, *allocator);

	config_document.AddMember(member_name, color_value, *allocator);
}

void Serialization::GetColor(const std::string& name, float4& return_value, const float4& opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return_value = opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return_value = float4(
			current_value[0].GetFloat(),
			current_value[1].GetFloat(),
			current_value[2].GetFloat(),
			current_value[3].GetFloat()
		);
	}
}

void Serialization::AddQuat(const Quat& value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	rapidjson::Value quat_value(rapidjson::kArrayType);
	quat_value.PushBack(value_to_add.x, *allocator);
	quat_value.PushBack(value_to_add.y, *allocator);
	quat_value.PushBack(value_to_add.z, *allocator);
	quat_value.PushBack(value_to_add.w, *allocator);

	config_document.AddMember(member_name, quat_value, *allocator);
}

void Serialization::GetQuat(const std::string& name, Quat& return_value, const Quat& opt_value) const
{
	if (!config_document.HasMember(name.c_str()))
	{
		return_value = opt_value;
	}
	else
	{
		const rapidjson::Value& current_value = config_document[name.c_str()];
		return_value = Quat(
			current_value[0].GetFloat(),
			current_value[1].GetFloat(),
			current_value[2].GetFloat(),
			current_value[3].GetFloat()
		);
	}
}

void Serialization::AddChildConfig(Serialization& value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	rapidjson::Value tmp_value(rapidjson::kObjectType);
	tmp_value.CopyFrom(value_to_add.config_document, *allocator);
	config_document.AddMember(member_name, tmp_value, *allocator);
}

void Serialization::GetChildConfig(const std::string& name, Serialization& value_to_add) const
{
	assert(config_document.HasMember(name.c_str()));

	const rapidjson::Value& tmp_value = config_document[name.c_str()];
	value_to_add = Serialization(tmp_value);
}

void Serialization::AddChildrenConfig(std::vector<Serialization> &value_to_add, const std::string& name)
{
	rapidjson::Value member_name(name.c_str(), *allocator);
	rapidjson::Value children_configs_value(rapidjson::kArrayType);
	for (unsigned int i = 0; i < value_to_add.size(); ++i)
	{
		rapidjson::Value tmp_value(rapidjson::kObjectType);
		tmp_value.CopyFrom(value_to_add[i].config_document.Move(), *allocator);
		children_configs_value.PushBack(tmp_value.Move(), *allocator);
	}
	config_document.AddMember(member_name, children_configs_value, *allocator);
}

void Serialization::GetChildrenConfig(const std::string& name, std::vector<Serialization>& return_value) const
{
	assert(config_document.HasMember(name.c_str()));

	const rapidjson::Value& children_configs_value = config_document[name.c_str()];
	return_value = std::vector<Serialization>();
	for (unsigned int i = 0; i < children_configs_value.Size(); ++i)
	{
		const rapidjson::Value& tmp_value = children_configs_value[i];
		Serialization tmp_config(tmp_value);
		return_value.push_back(tmp_config);
	}
}