#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Windows.h>

using Json = nlohmann::json;
using std::string;
using std::cout;
using std::endl;

int ChangeXXMIConfig(const char * server, const char * configFileName = "./config.json") {
    std:: ifstream configFile(configFileName);
    Json config = Json::parse(configFile);
    // cout << "config:: \n" << config.dump(4) << endl;
    // system("pause");
    string xxmiConfigPath = config["XXMI_HOME"].get<string>()+"\\XXMI Launcher Config.json";
    std:: ifstream xxmiConfigFile(xxmiConfigPath);
    Json xxmiConfig = Json::parse(xxmiConfigFile);
    // cout << "xxmiConfigPath:: " << xxmiConfigPath << endl;
    // system("pause");
    // cout << "xxmiConfig::\n" << xxmiConfig.dump(4) << endl;
    // system("pause");
    xxmiConfig["Importers"]["GIMI"]["Importer"]["game_folder"] = config["gameClientHome"][server]["directory"].get<string>();
    
    std:: ofstream xxmiConfigFileOutput(xxmiConfigPath);
    xxmiConfigFileOutput << xxmiConfig.dump(4,' ',true) << std::endl;
    return 0;
}

int main(int argc, char* argv[])
{
    string server = "CN";
    if(argc > 1) {
        server = argv[1];
    }
    if(server == "CN" || server == "Inter") {
        int result = ChangeXXMIConfig(server.c_str());
        return result;
    }
    else {
        std::cout << "[Error] ChangeXXMIConfig: 请输入正确的服务器参数: " << server << std::endl;
        return 1;
    }
    return 1;
}