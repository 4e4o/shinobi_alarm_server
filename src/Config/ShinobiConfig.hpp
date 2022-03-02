#ifndef SHINOBI_CONFIG_HPP
#define SHINOBI_CONFIG_HPP

#include <Config/ConfigItem.hpp>
#include <Config/ArrayParser.hpp>

class CameraConfig;

class ShinobiConfig : public ConfigItem, public ArrayParser {
public:
    typedef std::list<std::unique_ptr<CameraConfig>> TCameras;

    ShinobiConfig(const boost::json::object&);

    void init(const boost::json::object&) override final;

    const std::string& getId() const;
    const TCameras& getCameras() const;

private:
    std::string m_id;
    TCameras m_cameras;
};

#endif /* SHINOBI_CONFIG_HPP */
