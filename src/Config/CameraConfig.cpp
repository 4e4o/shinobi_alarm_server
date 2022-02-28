#include "CameraConfig.hpp"

#include <boost/json.hpp>

CameraConfig::CameraConfig(const boost::json::object &o) :
    m_name(toStdString(o.at("name").as_string())),
    m_camId(toStdString(o.at("shinobi_camera_id").as_string())),
    m_mappedId(o.at("mapped_id").as_int64()) {
}

const std::string& CameraConfig::name() const {
    return m_name;
}

const std::string& CameraConfig::camId() const {
    return m_camId;
}

int CameraConfig::mappedId() const {
    return m_mappedId;
}
