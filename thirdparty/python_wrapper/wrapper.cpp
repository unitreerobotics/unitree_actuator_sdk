#include <pybind11/pybind11.h> 
#include "unitreeMotor/unitreeMotor.h"
#include "serialPort/SerialPort.h"

namespace py = pybind11; 

PYBIND11_MODULE(unitree_actuator_sdk, m){ 
m.doc() = "unitree_actuator_sdk python wrapper"; 
 
py::enum_<MotorType>(m, "MotorType")
  .value("A1", MotorType::A1, "A1 Motor")
  .value("B1", MotorType::B1, "B1 Motor")
  .value("GO_M8010_6", MotorType::GO_M8010_6, "GO_M8010_6 Motor");

py::enum_<MotorMode>(m, "MotorMode")
  .value("BRAKE", MotorMode::BRAKE, "BRAKE mode")
  .value("FOC", MotorMode::FOC, "FOC")
  .value("CALIBRATE", MotorMode::CALIBRATE, "CALIBRATE mode");

py::class_<MotorCmd>(m, "MotorCmd") 
  .def(pybind11::init<>())
  .def_readwrite("motorType", &MotorCmd::motorType)
  .def_readwrite("hex_len", &MotorCmd::hex_len)
  .def_readwrite("id", &MotorCmd::id)
  .def_readwrite("mode", &MotorCmd::mode)
  .def_readwrite("tau", &MotorCmd::tau)
  .def_readwrite("dq", &MotorCmd::dq)
  .def_readwrite("q", &MotorCmd::q)
  .def_readwrite("kp", &MotorCmd::kp)
  .def_readwrite("kd", &MotorCmd::kd);

py::class_<MotorData>(m, "MotorData") 
  .def(pybind11::init<>())
  .def_readwrite("motorType", &MotorData::motorType)
  .def_readwrite("hex_len", &MotorData::hex_len)
  .def_readwrite("motor_id", &MotorData::motor_id)
  .def_readwrite("mode", &MotorData::mode)
  .def_readwrite("temp", &MotorData::temp)
  .def_readwrite("merror", &MotorData::merror)
  .def_readwrite("tau", &MotorData::tau)
  .def_readwrite("dq", &MotorData::dq)
  .def_readwrite("q", &MotorData::q)
  .def_readwrite("correct", &MotorData::correct);

py::class_<SerialPort>(m, "SerialPort")
  .def(py::init<const std::string &>())
  .def("test", &SerialPort::test)
  .def("sendRecv", py::overload_cast<MotorCmd*, MotorData*>(&SerialPort::sendRecv));

m.def("queryMotorMode", &queryMotorMode, "Query Motor Mode");
m.def("queryGearRatio", &queryGearRatio, "Query Motor Gear Ratio");
}