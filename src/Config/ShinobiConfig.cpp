#include "ShinobiConfig.hpp"
#include "CameraConfig.hpp"

#include <boost/json.hpp>

ShinobiConfig::ShinobiConfig(const boost::json::object &o) :
    m_id(get<std::string>(o, "shinobi_account_id")) {
    registerType<ConfigItem, CameraConfig, const boost::json::object&>();
}

void ShinobiConfig::init(const boost::json::object& o) {
    if (!o.contains("cameras"))
        return;

    const auto& cams = o.at("cameras");
    ArrayParser::TItems camsItems;
    parseArray(cams.as_array(), camsItems);
    for (auto& c : camsItems) {
        m_cameras.emplace_back(static_cast<CameraConfig*>(c.release()));
    }
}

const std::string& ShinobiConfig::getId() const {
    return m_id;
}

const ShinobiConfig::TCameras& ShinobiConfig::getCameras() const {
    return m_cameras;
}
