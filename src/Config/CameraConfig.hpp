#ifndef CAMERA_CONFIG_HPP
#define CAMERA_CONFIG_HPP

#include <Config/ConfigItem.hpp>

class CameraConfig : public ConfigItem {
public:
    CameraConfig(const boost::json::object&);

    const std::string& name() const;
    const std::string& camId() const;
    int mappedId() const;

private:
    std::string m_name;
    std::string m_camId;
    int m_mappedId;
};

#endif /* CAMERA_CONFIG_HPP */
