/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#ifndef BASECHANNELREQUESTER_H_
#define BASECHANNELREQUESTER_H_

#ifdef epicsExportSharedSymbols
#   define baseChannelRequesterEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/requester.h>
#include <pv/destroyable.h>

#ifdef baseChannelRequesterEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef baseChannelRequesterEpicsExportSharedSymbols
#endif

#include <pv/serverContext.h>
#include <pv/serverChannelImpl.h>

namespace epics {
namespace pvAccess {

class BaseChannelRequester :  virtual public epics::pvData::Requester, public epics::pvData::Destroyable
{
public:
    BaseChannelRequester(ServerContextImpl::shared_pointer const & context, ServerChannelImpl::shared_pointer const & channel,
                         const pvAccessID ioid, Transport::shared_pointer const & transport);
    virtual ~BaseChannelRequester() {};

    bool startRequest(epics::pvData::int32 qos);
    void stopRequest();
    epics::pvData::int32 getPendingRequest();
    std::string getRequesterName();
    void message(std::string const & message, epics::pvData::MessageType messageType);
    static void message(Transport::shared_pointer const & transport, const pvAccessID ioid, const std::string message, const epics::pvData::MessageType messageType);
    static void sendFailureMessage(const epics::pvData::int8 command, Transport::shared_pointer const & transport, const pvAccessID ioid, const epics::pvData::int8 qos, const epics::pvData::Status status);

    static const epics::pvData::Status okStatus;
    static const epics::pvData::Status badCIDStatus;
    static const epics::pvData::Status badIOIDStatus;
    static const epics::pvData::Status noReadACLStatus;
    static const epics::pvData::Status noWriteACLStatus;
    static const epics::pvData::Status noProcessACLStatus;
    static const epics::pvData::Status otherRequestPendingStatus;
    static const epics::pvData::Status notAChannelRequestStatus;
protected:
    const pvAccessID _ioid;
    Transport::shared_pointer _transport;
    ServerChannelImpl::shared_pointer _channel;
    epics::pvData::Mutex _mutex;
private:
    ServerContextImpl::shared_pointer _context;
    static const epics::pvData::int32 NULL_REQUEST;
    epics::pvData::int32 _pendingRequest;
};

class BaseChannelRequesterMessageTransportSender : public TransportSender
{
public:
    BaseChannelRequesterMessageTransportSender(const pvAccessID _ioid, const std::string message,const epics::pvData::MessageType messageType);
    void send(epics::pvData::ByteBuffer* buffer, TransportSendControl* control);
    void lock();
    void unlock();
private:
    const pvAccessID _ioid;
    const std::string _message;
    const epics::pvData::MessageType _messageType;
};

class BaseChannelRequesterFailureMessageTransportSender : public TransportSender
{
public:
    BaseChannelRequesterFailureMessageTransportSender(const epics::pvData::int8 command, Transport::shared_pointer const & transport, const pvAccessID ioid, const epics::pvData::int8 qos, const epics::pvData::Status& status);
    void send(epics::pvData::ByteBuffer* buffer, TransportSendControl* control);
    void lock();
    void unlock();

private:
    const epics::pvData::int8 _command;
    const pvAccessID _ioid;
    const epics::pvData::int8 _qos;
    const epics::pvData::Status _status;
    Transport::shared_pointer _transport;
};

}
}


#endif /* BASECHANNELREQUESTER_H_ */
