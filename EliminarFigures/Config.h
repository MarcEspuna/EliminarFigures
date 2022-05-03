#include <string>

namespace Config
{
	struct Lighting
	{
		float lightDir[3];
		int ambient, diffuse, specular, shinnines;
		bool highlight;
	};

	struct Movement
	{
		int rotation;
		bool randPositionning;
	};

	struct Sockets
	{
		int	aiPort;
		int	userXPort;
		int	userYPort;
		bool aiEnable;
		bool userXEnable;
		bool userYEnable;
	};

	struct Obj
	{
		std::string filenames[5];
		std::string description[5];
		int scale[5];
		Lighting light;
		Movement movement;
	};

	struct Config
	{
		bool aiPlayerX, aiPlayerY;
		Obj obj;
		Sockets socket;
	};

	// Method is called every time we convert form object -> json
	static void to_json(nlohmann::json& j, const Config& p)
	{
		//TODO
	}

	static void from_json(const nlohmann::json& j, Movement& p)
	{
		j.at("rotation").get_to(p.rotation);
		j.at("random positioning").get_to(p.randPositionning);
	}

	static void from_json(const nlohmann::json& j, Lighting& p)
	{
		j.at("ambient").get_to(p.ambient);
		j.at("diffuse").get_to(p.diffuse);
		j.at("specular").get_to(p.specular);
		j.at("light direction").get_to(p.lightDir);
		j.at("shinnines").get_to(p.shinnines);
		j.at("highlight").get_to(p.highlight);
	}

	// Note: get_to also calls from json so we can recursively go down the structs
	static void from_json(const nlohmann::json& j, Obj& p)
	{
		j.at("filenames").get_to(p.filenames);			// Carefull to not exceed 5 objects in the json file
		j.at("descriptions").get_to(p.description);		// Carefull to not exceed 5 objects in the json file
		j.at("scale").get_to(p.scale);					// Carefull to not exceed 5 objects in the json file
		j.at("lighting").get_to(p.light);
		j.at("movement").get_to(p.movement);
	}

	static void from_json(const nlohmann::json& j, Sockets& p)
	{
		j.at("aiPort").get_to(p.aiPort);
		j.at("aiEnable").get_to(p.aiEnable);
		j.at("userXPort").get_to(p.userXPort);
		j.at("userYPort").get_to(p.userYPort);
		j.at("userXEnable").get_to(p.userXEnable);
		j.at("userYEnable").get_to(p.userYEnable);
	}

	// Method is called every time we convert from json -> object
	static void from_json(const nlohmann::json& j, Config& p)
	{
		j.at("AiPlayerX").get_to(p.aiPlayerX);
		j.at("AiPlayerY").get_to(p.aiPlayerY);
		j.at("object").get_to(p.obj);
		j.at("sockets").get_to(p.socket);
	}
}
