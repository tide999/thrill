/*******************************************************************************
 * c7a/communication/system_control_channel.cpp
 *
 ******************************************************************************/

#include <c7a/communication/system_control_channel.hpp>

namespace c7a {

void MasterSystemControlChannel::setCallback(MasterControlCallback callback)
{ }

void MasterSystemControlChannel::rollBackStage()
{ }

void MasterSystemControlChannel::abortExecution()
{ }

void WorkerSystemControlChannel::setCallback(WorkerControlCallback callback)
{ }

void WorkerSystemControlChannel::requestBackupLocation()
{ }

void WorkerSystemControlChannel::notifyBackupComplete()
{ }

} // namespace c7a

/******************************************************************************/
