#ifndef DISEASETRACKOBJECT_H
#define DISEASETRACKOBJECT_H

#include "sigslot.h"
#include <ctime>
#include <string>
#include <vector>

enum STATE { NONE = 0, READY = 1, RUNNING = 2, FAILURE = 3 };

class DiseaseTrackObject : public sigslot::has_slots<> {
public:
  // Methods
  DiseaseTrackObject();
  virtual ~DiseaseTrackObject() = 0;
  // Members
  // Signals
  sigslot::signal3<std::string, std::string, std::string> LogMessage;
  sigslot::signal1<int> ForwardRunModeOrder;
  sigslot::signal1<int> PublishRunMode;
  // Slots
  void AcceptLogMessagePassthrough(std::string, std::string, std::string);
  virtual void AcceptRunModeOrder(int);

private:
  // Methods
  virtual void init();
  virtual void pause();
  virtual void run();
  virtual const std::string identify();
  // Members
  // Signals
  // Slots
protected:
  // Methods
  void logMessage(std::string, std::string);
  virtual void forwardRunModeOrder(int);
  void connectChild(DiseaseTrackObject *);
  void disconnectChild(DiseaseTrackObject *);
  // Members
  clock_t m_timeofcreation;
  STATE m_state;
  std::vector<DiseaseTrackObject *> m_children;
  // Signals
  // Slots
};

#endif // DISEASETRACKOBJECT_H
