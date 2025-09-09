#pragma once

#include <CommonAPI/CommonAPI.hpp>
#include <v1/gnss/GnssServerProxy.hpp>
#include "std_msgs/msg/string.hpp"

#include <types/conversion.h>
#include <optional>

#include<iostream>
#include<fstream>



using GpsDataMsg = std_msgs::msg::String;
using GnssData = v1::gnss::common::Str;

template<template<typename ...> class P>
class AbstractSomeIpClient 
{
    using ProxyClass = P<>;
    using ProxyClassPtr = std::shared_ptr<ProxyClass>;

public: 
    AbstractSomeIpClient(std::string domain, std::string instance) 
        : proxy_(CommonAPI::Runtime::get()->buildProxy<P>(domain,instance)) 
    {
        init();
    }

    virtual std::optional<bool> available() {
        return (initialised()) ? std::make_optional(proxy_->isAvailable()) : std::nullopt;
    }

    virtual void onAvailable() = 0;

protected:
    ProxyClassPtr proxy() {
        return proxy_;
    }

    virtual bool initialised() {
        return proxy_ != nullptr; 
    }

    void init() {
        if(!proxy_)
        {
            return;
        }

        proxy_->getProxyStatusEvent().subscribe(std::bind(&AbstractSomeIpClient<P>::onAvailablilityStatusChange, this, std::placeholders::_1));
    }

    virtual void onAvailablilityStatusChange(CommonAPI::AvailabilityStatus status) {

        if (status == CommonAPI::AvailabilityStatus::AVAILABLE)
        {
            onAvailable();
        }
    }

private:
    ProxyClassPtr proxy_;
};

using GnssSomeIpProxyWrapper = AbstractSomeIpClient<v1::gnss::GnssServerProxy>;

class GnssSomeIpClient : public GnssSomeIpProxyWrapper
{
    static constexpr auto domain = "local";
    static constexpr auto instance = "GnssServer";


    using MessageCallback = std::function<void(const GpsDataMsg & message)>;

public:
    GnssSomeIpClient() : GnssSomeIpProxyWrapper(domain, instance) {}

    void setMessageCallback(MessageCallback callback) {
        message_callback = std::move(callback);
    }

    

    void onAvailable() override {


            proxy()->getDataEvent().subscribe([this](const GnssData & data) {



            auto message = Types::Conversion::from_capi_type(data);
            // std::cout << message.data << std::endl;
            
            message_callback(message);
        });
    }

private:
    MessageCallback message_callback;
};
