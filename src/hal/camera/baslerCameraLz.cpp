#include "baslerCameraLz.h"
#include "baslerConfigurationEvent.h"
#include "baslerImageEvent.h"
#include <QTimer>

namespace TIGER_BaslerCamera
{
    const char cCameraNodeMapFile[] = "./GigECameraNodeMap.pfs";
    IPylonDevice *getYcgCameraDevice(const QString &p_ip)
    {
        IPylonDevice *pGigECameraDevice = nullptr;
        CTlFactory &tlFactory = CTlFactory::GetInstance();
        DeviceInfoList_t devices;
        for (int i = 0; i < tlFactory.CreateTl("BaslerGigE")->EnumerateDevices(devices); i++)
        {
            CDeviceInfo cameraInfo = devices[i];
            trace(" Using device: " << cameraInfo.GetModelName());
            trace("Friendly Name: " << cameraInfo.GetFriendlyName());
            trace("    Full Name: " << cameraInfo.GetFullName());
            trace(" SerialNumber: " << cameraInfo.GetSerialNumber());
            QString ip = cameraInfo.GetAddress();
            trace("    camera IP: " << ip);
            ip = ip.left(ip.indexOf(':'));
            if (ip == p_ip)
            {
                pGigECameraDevice = tlFactory.CreateDevice(cameraInfo);
                break;
            }
        }
        return pGigECameraDevice;
    }

    SINGLETON_GARBO(CBaslerCameraLz);
    CBaslerCameraLz::CBaslerCameraLz()
        : m_pGigECamera(nullptr),
          m_ip("169.254.169.99"),
          m_exposureTime(30000.0),
          m_heartbeatTimeout(1000)
    {
        PylonInitialize();
        m_pGigECamera = new CBaslerGigEInstantCamera(nullptr);
        CImageEvent *sampleImageEventHandler = new CImageEvent;
        connect(sampleImageEventHandler, &CImageEvent::sigGrabImage, this, &CBaslerCameraLz::slotGrabImage);
        m_pGigECamera->RegisterImageEventHandler(sampleImageEventHandler, RegistrationMode_ReplaceAll, Cleanup_Delete);

        CConfigurationEvent *configurationEventHandler = new CConfigurationEvent;
        connect(configurationEventHandler, &CConfigurationEvent::sigGrabError, this, &CBaslerCameraLz::slotPrintError);
        connect(configurationEventHandler, &CConfigurationEvent::sigCameraDisconnected, this, &CBaslerCameraLz::slotDisconnected);
        m_pGigECamera->RegisterConfiguration(configurationEventHandler, RegistrationMode_ReplaceAll, Cleanup_Delete);
        slotDisconnected();
    }

    CBaslerCameraLz::~CBaslerCameraLz()
    {
        closeCamera();
        delPtr(m_pGigECamera);
        PylonTerminate();
    }
    void CBaslerCameraLz::setHeartbeatTimeout(const int &p_dMillisecond)
    {
        if (p_dMillisecond != m_heartbeatTimeout)
        {
            m_heartbeatTimeout = p_dMillisecond;
        }
    }

    int CBaslerCameraLz::getHeartbeatTimeout() const
    {
        return m_heartbeatTimeout;
    }

    void CBaslerCameraLz::setExposureTime(const double &p_time)
    {
        if (m_exposureTime != p_time)
        {
            m_exposureTime = p_time;
        }
    }

    double CBaslerCameraLz::getExposureTime() const
    {
        return m_exposureTime;
    }

    bool CBaslerCameraLz::isAcquire()
    {
        return connected() && m_pGigECamera->IsGrabbing();
    }

    void CBaslerCameraLz::setIP(const QString &p_ip)
    {
        m_ip = p_ip;
    }

    QString CBaslerCameraLz::getIp() const
    {
        return m_ip;
    }

    QString CBaslerCameraLz::getLastError() const
    {
        return m_error;
    }

    QImage CBaslerCameraLz::getLastImage() const
    {
        return m_lastImage;
    }

    bool CBaslerCameraLz::connected()
    {
        if (m_isConnected)
        {
            return true;
        }
        try
        {
            QString ip = m_pGigECamera->GetDeviceInfo().GetAddress();
            if (m_pGigECamera->IsPylonDeviceAttached())
            {
                m_pGigECamera->DetachDevice();
            }
            else
            {
                closeCamera();
                if (ip.left(ip.indexOf(':')) == m_ip)
                {
                    return true;
                }
            }
            IPylonDevice *device = getYcgCameraDevice(m_ip);
            if (device == nullptr)
            {
                slotPrintError("initial Error:notfind  IP " + m_ip);
                return false;
            }
            m_pGigECamera->Attach(device, Cleanup_Delete);
            return true;
        }
        catch (GenICam::GenericException &e)
        {
            slotPrintError("initial Error:" + QString(e.GetDescription()));
        }
        return false;
    }

    bool CBaslerCameraLz::singleFrame()
    {
        assert(connected());
        try
        {
            if (m_pGigECamera->IsGrabbing())
            {
                slotPrintError(cnStr("相机正在采集,请停止..."));
                return false;
            }
            if (!openCamera())
            {
                slotPrintError(cnStr("相机打开失败，请检查相机..."));
                return false;
            }
            CGrabResultPtr ptrGrabResult;
            m_pGigECamera->StartGrabbing(1, GrabStrategy_LatestImageOnly);
            bool isSuccessful = m_pGigECamera->RetrieveResult(5000, ptrGrabResult, TimeoutHandling_Return);
            closeCamera();
            if (!isSuccessful)
            {
                slotPrintError(cnStr("抓取图片失败:%1").arg(m_pGigECamera->LastError.ToString().c_str()));
            }
            return isSuccessful;
        }
        catch (GenICam::GenericException &e)
        {
            slotPrintError("singleFrame Error:" + QString(e.GetDescription()));
            return false;
        }
        return false;
    }

    bool CBaslerCameraLz::acquireChange()
    {
        assert(connected());
        try
        {
            if (m_pGigECamera->IsGrabbing())
            {
                m_pGigECamera->StopGrabbing();
                emit sigAcquireStateChanged();
                return closeCamera();
            }
            if (!openCamera())
            {
                return false;
            }
            m_pGigECamera->StartGrabbing(GrabStrategy_OneByOne, GrabLoop_ProvidedByInstantCamera);
            emit sigAcquireStateChanged();
            return true;
        }
        catch (GenICam::GenericException &e)
        {
            slotPrintError("Acquire Error:" + QString(e.GetDescription()));
        }
        return false;
    }

    void CBaslerCameraLz::slotPrintError(const QString &p_msg)
    {
        m_error = p_msg;
        closeCamera();
    }

    bool CBaslerCameraLz::openCamera()
    {
        bool isSuccessful = true;
        try
        {
            if (!m_pGigECamera->IsOpen() && !m_pGigECamera->IsCameraDeviceRemoved())
            {
                m_pGigECamera->Open();
                // CFeaturePersistence::Save(cCameraNodeMapFile, &m_pGigECamera->GetNodeMap());

#define setPara(Node, value)                       \
    if (GenApi::IsWritable(m_pGigECamera->##Node)) \
    {                                              \
        m_pGigECamera->##Node.SetValue(value);     \
    }
                setPara(TriggerMode, Basler_GigECamera::TriggerMode_Off);
                setPara(ExposureAuto, Basler_GigECamera::ExposureAuto_Off);
                m_pGigECamera->ExposureTimeAbs = m_exposureTime;
                setPara(GevHeartbeatTimeout, m_heartbeatTimeout);
                setPara(AcquisitionMode, Basler_GigECamera::AcquisitionMode_Continuous);
#undef setPara
            }
        }
        catch (GenICam::GenericException &e)
        {
            slotPrintError("openCamera Error:" + QString(e.GetDescription()));
            isSuccessful = false;
        }
        return isSuccessful;
    }

    void CBaslerCameraLz::slotGrabImage(QImage p_image)
    {
        m_lastImage = p_image;
        emit sigGrabImage(m_lastImage);
    }

    void CBaslerCameraLz::slotDisconnected()
    {
        m_isConnected = false;
        emit sigAcquireStateChanged();
        if (connected())
        {
            m_isConnected = true;
            return;
        }
        // qInfo() << cnStr("相机连接失败:%1,5s后尝试连接相机").arg(getLastError());
        QTimer::singleShot(5000, this, &CBaslerCameraLz::slotDisconnected);
    }

    bool CBaslerCameraLz::closeCamera()
    {
        bool isSuccessful = true;
        try
        {
            if (m_pGigECamera->IsGrabbing())
            {
                m_pGigECamera->StopGrabbing();
            }
            if (m_pGigECamera->IsOpen())
            {
                m_pGigECamera->Close();
            }
        }
        catch (GenICam::GenericException &e)
        {
            slotPrintError("closeCamera Error:" + QString(e.GetDescription()));
            isSuccessful = false;
        }
        return isSuccessful;
    }
};

TIGER_BaslerCamera::CBaslerCameraLz *baslerCameraLz()
{
    return TIGER_BaslerCamera::CBaslerCameraLz::instance();
}
