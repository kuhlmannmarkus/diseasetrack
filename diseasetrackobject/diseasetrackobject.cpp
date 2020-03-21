#include "diseasetrackobject.h"

DiseaseTrackObject::DiseaseTrackObject() {
  m_state = NONE;
  m_timeofcreation = clock();
  m_children.clear();
}

DiseaseTrackObject::~DiseaseTrackObject() {}

const std::string DiseaseTrackObject::identify() {
  return std::string("DiseaseTrackObject");
}

void DiseaseTrackObject::logMessage(std::string _message, std::string _severity) {
  LogMessage.emit(identify(), _message, _severity);
  return;
}

void DiseaseTrackObject::AcceptLogMessagePassthrough(std::string _from,
                                                 std::string _message,
                                                 std::string _severity) {
  LogMessage.emit(_from, _message, _severity);
  return;
}

void DiseaseTrackObject::connectChild(DiseaseTrackObject *_child) {
  m_children.push_back(_child);
  _child->LogMessage.connect(dynamic_cast<DiseaseTrackObject *>(this),
                             &DiseaseTrackObject::AcceptLogMessagePassthrough);
  ForwardRunModeOrder.connect(dynamic_cast<DiseaseTrackObject *>(_child),
                              &DiseaseTrackObject::AcceptRunModeOrder);
  return;
}

void DiseaseTrackObject::disconnectChild(DiseaseTrackObject *_child) {
  _child->LogMessage.disconnect(dynamic_cast<DiseaseTrackObject *>(this));
  ForwardRunModeOrder.disconnect(dynamic_cast<DiseaseTrackObject *>(_child));
  return;
}

void DiseaseTrackObject::init() { return; }

void DiseaseTrackObject::pause() { return; }

void DiseaseTrackObject::run() { return; }

void DiseaseTrackObject::AcceptRunModeOrder(int _in) {
  forwardRunModeOrder(_in);
  return;
}

void DiseaseTrackObject::forwardRunModeOrder(int _in) {
  ForwardRunModeOrder.emit(_in);
  return;
}
