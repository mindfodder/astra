#ifndef DEVICEADAPTER_H
#define DEVICEADAPTER_H

#include <SenseKit.h>

namespace sensekit {

    enum driver_status_t
        {
            DRIVER_STATUS_SUCCESS
        };

    class DriverAdapter;

    typedef void* device_handle_t;
    typedef void (*device_connected_callback_t)(DriverAdapter* adapter, const sensekit_device_desc_t& desc, void* callbackContext);
    typedef void (*device_disconnected_callback_t)(const sensekit_device_desc_t& desc, void* callbackContext);
    typedef void (*device_changed_callback_t)(const sensekit_device_desc_t& desc, void* callbackContext);

    class DriverAdapter
    {
    public:
        DriverAdapter() {};
        virtual ~DriverAdapter() {}

        virtual sensekit_status_t initialize(
            device_connected_callback_t connectedCallback,
            device_disconnected_callback_t disconnectedCallback,
            device_changed_callback_t changedCallback,
            void* callbackContext)
            {
                m_deviceConnectedCallback = connectedCallback;
                m_deviceDisconnectedCallback = disconnectedCallback;
                m_deviceChangedCallback = changedCallback;
                m_callbackContext = callbackContext;

                return SENSEKIT_STATUS_SUCCESS;
};
        virtual sensekit_status_t terminate() = 0;
        virtual device_handle_t open_device(char* uri) = 0;
        virtual driver_status_t close_device(device_handle_t handle) = 0;
        virtual sensekit_status_t has_device_for_uri(char *uri, bool& deviceAvailable) = 0;

    protected:
        device_connected_callback_t m_deviceConnectedCallback;
        device_disconnected_callback_t m_deviceDisconnectedCallback;
        device_changed_callback_t m_deviceChangedCallback;
        void* m_callbackContext;

    private:

    };
}

#endif /* DEVICEADAPTER_H */