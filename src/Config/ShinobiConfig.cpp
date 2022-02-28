#include "ShinobiConfig.hpp"
#include "CameraConfig.hpp"

#include <boost/json.hpp>

ShinobiConfig::ShinobiConfig(const boost::json::object &o) :
    m_id(toStdString(o.at("shinobi_account_id").as_string())) {

    if (!o.contains("cameras"))
        return;

    const auto& cams = o.at("cameras");
    registerType<ConfigItem, CameraConfig, const boost::json::object&>();
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
