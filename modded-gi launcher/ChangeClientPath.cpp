#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <Windows.h>

using Json = nlohmann::json;
using std::string;
using std::cout;
using std::endl;

const string configPath = ".\\config.json";

int ChangeXXMIConfig(const Json& config, const string& server) {
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

int ChangeFpsUnlockerConfig(const Json& config, const string& server) {
    string fpsUnlockerConfigPath = config["XXMI_HOME"].get<string>()+"\\Resources\\Packages\\GI-FPS-Unlocker\\fps_config.json";
    std::ifstream fpsUnlockerConfigFile(fpsUnlockerConfigPath);
    if( !fpsUnlockerConfigFile) {
        std:: cerr << "[log] ChangeClientPath: 未找到fpsUnlocker的配置，更改取消。" << endl;
        return 0;
    }
    Json fpsConfig = Json::parse(fpsUnlockerConfigFile);
    string clientName = (server == "CN") ? "YuanShen.exe" : "GenshinImpact.exe";
    fpsConfig["GamePath"] = config["gameClientHome"][server]["directory"].get<string>()+clientName;

    std:: ofstream fpsConfigOutput(fpsUnlockerConfigPath);
    fpsConfigOutput << fpsConfig.dump(4,' ',false) << endl;
    return 0;
}

int main(int argc, char* argv[])
{
    string server = "CN";
    if(argc > 1) {
        server = argv[1];
    }
    std::ifstream configFile(configPath);
    Json config = Json::parse(configFile);
    if(server == "CN" || server == "Inter") {
        int result = ChangeXXMIConfig(config, server);
        int result1 = ChangeFpsUnlockerConfig(config, server);
        result |= result1;
        return result;
    }
    else {
        std::cout << "[Error] ChangeXXMIConfig: 请输入正确的服务器参数: " << server << std::endl;
        return 1;
    }
    return 1;
}