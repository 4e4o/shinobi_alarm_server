#include "CameraConfig.hpp"

#include <boost/json.hpp>

CameraConfig::CameraConfig(const boost::json::object &o) :
    m_name(get<std::string>(o, "name")),
    m_camId(get<std::string>(o, "shinobi_camera_id")),
    m_mappedId(get<std::int64_t>(o, "mapped_id")) {
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
