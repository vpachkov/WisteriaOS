#include "DeviceManager.hpp"
#include "PTY/PTMX.hpp"
#include <Devices/Drivers/Driver.hpp>
#include <Libkernel/Assert.hpp>
#include <Libkernel/Logger.hpp>

namespace Kernel::Devices {

void DeviceManager::register_virtual_devices()
{
    register_device(ptmx);
}

void DeviceManager::acknowledge_driver(Driver* driver)
{
    m_acknowledged_drivers[driver->driver_name()] = driver;
}

void DeviceManager::install_acknowledged_drivers()
{
    static char* desired_drivers[] = {
        "VgaTUI",
        "UART",
        "ATA",
        "PS/2-Mouse",
        "PS/2-Keyboard",
        "RTC",
        "PIT",
        "PCI",
        "BGA",
    };

    for (auto desired_driver : desired_drivers) {
        if (m_acknowledged_drivers.contains(desired_driver)) {
            m_acknowledged_drivers[desired_driver]->install();
            m_attached_drivers[desired_driver] = m_acknowledged_drivers[desired_driver];
            Log() << "[DeviceManager] Installed a driver: " << desired_driver << "\n";
            continue;
        }

        Log() << "[DeviceManager] Failed to install a driver: " << desired_driver << "\n";
    }
}

Driver* DeviceManager::find_attached_driver_by_name(const String& name)
{
    if (m_attached_drivers.contains(name)) {
        return m_attached_drivers[name];
    }
    return nullptr;
}

}