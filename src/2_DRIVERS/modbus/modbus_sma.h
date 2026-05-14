/**
 * @file modbus_sma.h
 * @brief SMA MODBUS protocol registers
 */

#ifndef MODBUS_SMA_H__
#define MODBUS_SMA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/** SMA Data types NaN values */
typedef enum smaModbus_dataNaNValues {
    eSmaModbusDataNaNS16 = 0x8000,
    eSmaModbusDataNaNS32 = 0x80000000,
    eSmaModbusDataNaNSTR32 = 0,
    eSmaModbusDataNaNU16 = 0xFFFF,
    eSmaModbusDataNaNU32 = 0xFFFFFFFF,
    eSmaModbusDataNaNU32Status = 0xFFFFFD,
    eSmaModbusDataNaNU64 = 0xFFFFFFFFFFFFFFFF,
} smaModbus_dataNaNValues_t;

/** SMA Data Formats */
typedef unsigned long Duration;
typedef unsigned long DT;
typedef double FIX0;
typedef double FIX1;
typedef double FIX2;
typedef double FIX3;
typedef unsigned HW;
typedef uint8_t IP4[16];
typedef double RAW;
typedef double TEMP;
typedef unsigned long TM;

/** @brief The SMA MODBUS-assigned registers */
typedef enum smaModbus_registers {
    /** U32 RAW */
    eSmaModbusRegister_versionNumberOfTheSmaModbusProfile = 30001U,
    /** U32 RAW */
    eSmaModbusRegister_susyId = 30003U,
    /** U32 RAW */
    eSmaModbusRegister_serialNumber = 30005U,
    /** U32 RAW */
    eSmaModbusRegister_modbusDataChangeCounter = 30007U,
    /** U32 See \p smaModbus_deviceClass_e */
    eSmaModbusRegister_deviceClass = 30051U,
    /** [UNDOCUMENTED] */
    eSmaModbusRegister_numericalIdentificationOftheDeviceType = 0x30053U,
    /** U32 See \p smaModbus_manufacturerSpecification_e */
    eSmaModbusRegister_manufacturerSpecification = 30055U,
    /** U32 RAW */
    eSmaModbusRegister_serialNumber2 = 30057U,
    /** U32 FW */
    eSmaModbusRegister_softwarePackage = 30059U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfTheCurrentEvent = 30197U,
    /** U32 FIX0 */
    eSmaModbusRegister_currentCompleteEventNumber = 30247U,
    /** U32 Duration */
    eSmaModbusRegister_timeUntilGridConnectionAttempt = 30199U,
    /** U32 See \p smaModbus_statusOfTheDevice_e */
    eSmaModbusRegister_statusOfDevice = 30201U,
    /** U32 FIX0 */
    eSmaModbusRegister_powerInOkStatus = 30203U,
    /** U32 FIX0 */
    eSmaModbusRegister_powerInWarningStatus = 30205U,
    /** U32 FIX0 */
    eSmaModbusRegister_powerInErrorStatus = 30207U,
    /** U32 See \p smaModbus_recommendedAction_e */
    eSmaModbusRegister_recommendedAction = 30211U,
    /** U32 See \p smaModbus_statusMessage_e */
    eSmaModbusRegister_statusMessage = 30213U,
    /** U32 See \p smaModbus_statusDescription_e */
    eSmaModbusRegister_statusDescription = 30215U,
    /** U32 See \p smaModbus_utilityGridContactor_e */
    eSmaModbusRegister_utilityGridContactor = 30217U,
    /** U32 See \p smaModbus_temperatureDerating_e */
    eSmaModbusRegister_temperatureDerating = 30219U,
    /** U32 FIX0 */
    eSmaModbusRegister_insulationResistance = 30225U,
    /** U32 See \p smaModbus_statusOfKeySwitch_e */
    eSmaModbusRegister_statusOfKeySwitch = 30227U,
    /** U32 DT */
    eSmaModbusRegister_localTimeOfDevice = 30229U,
    /** U32 FIX0 */
    eSmaModbusRegister_maximumPossiblePermanentActivePower = 30231U,
    /** U32 FIX0 */
    eSmaModbusRegister_permanentActivePowerLimitation = 30233U,
    /** U32 See \p smaModbus_backupModeStatus_e */
    eSmaModbusRegister_backupModeStatus = 30235U,
    /** U32 See \p smaModbus_gridType_e */
    eSmaModbusRegister_gridType = 30237U,
    /** U32 See \p smaModbus_statusOfTheGfdiRelay_e */
    eSmaModbusRegister_statusOfTheGfdiRelay = 30249U,
    /** U32 See \p smaModbus_statusOfCurrentRestartInterlock_e */
    eSmaModbusRegister_statusOfCurrentRestartInterlock = 30251U,
    /** U32 See \p smaModbus_stateOfDcSwitch_e */
    eSmaModbusRegister_stateOfDcSwitch = 30257U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch1 = 30267U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch2 = 30269U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch3 = 30271U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch4 = 30273U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch5 = 30275U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch6 = 30277U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch7 = 30279U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch8 = 30281U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch9 = 30283U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch10 = 30285U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch11 = 30287U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch12 = 30289U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch13 = 30291U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch14 = 30293U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch15 = 30295U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch16 = 30297U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch17 = 30299U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch18 = 30301U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch19 = 30303U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch20 = 30305U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch21 = 30307U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch22 = 30309U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch23 = 30311U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch24 = 30313U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch25 = 30315U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch26 = 30317U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch27 = 30319U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch28 = 30321U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch29 = 30323U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch30 = 30325U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch31 = 30327U,
    /** U32 See \p smaModbus_dcSwitch_e */
    eSmaModbusRegister_dcSwitch32 = 30329U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch1 = 30331U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch2 = 30333U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch3 = 30335U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch4 = 30337U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch5 = 30339U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch6 = 30341U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch7 = 30343U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch8 = 30345U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch9 = 30347U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch10 = 30349U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch11 = 30351U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch12 = 30353U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch13 = 30355U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch14 = 30357U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch15 = 30359U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch16 = 30361U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch17 = 30363U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch18 = 30365U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch19 = 30367U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch20 = 30369U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch21 = 30371U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch22 = 30373U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch23 = 30375U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch24 = 30377U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch25 = 30379U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch26 = 30381U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch27 = 30383U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch28 = 30385U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch29 = 30387U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch30 = 30389U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch31 = 30391U,
    /** U32 See \p smaModbus_errorMessageDcSwitch_e */
    eSmaModbusRegister_errorMessageDcSwitch32 = 30393U,
    /** U64 FIX0 */
    eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors = 30513U,
    /** U64 FIX0 */
    eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors = 30517U,
    /** U64 Duration */
    eSmaModbusRegister_operatingTime = 30521U,
    /** U64 Duration */
    eSmaModbusRegister_feedInTime = 30525U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_Wh = 30529U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_kWh = 30531U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalAcEnergyFedInOnAllLineConductors_MWh = 30533U,
    /** U32 FIX0 */
    eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors_Wh = 30535U,
    /** U32 FIX0 */
    eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors_kWh = 30537U,
    /** U32 FIX0 */
    eSmaModbusRegister_energyFedInOnTheCurrentDayOnAllLineConductors_MWh = 30539U,
    /** U64 Duration */
    eSmaModbusRegister_operatingTime2 = 30541U,
    /** U64 Duration */
    eSmaModbusRegister_feedInTime2 = 30543U,
    /** U32 Duration */
    eSmaModbusRegister_operatingTimeOfInteriorFan1_U32 = 30545U,
    /** U32 Duration */
    eSmaModbusRegister_operatingTimeOfInteriorFan2_U32 = 30547U,
    /** U32 Duration */
    eSmaModbusRegister_operatingTimeOfHeatSinkFan_U32 = 30549U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfEventsAtUserLevel = 30559U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfEventsAtInstallerLevel = 30561U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfEventsAtServiceLevel = 30563U,
    /* U32 FIX0 */
    eSmaModbusRegister_numberOfGeneratorStarts = 30565U,
    /** U32 FIX0 */
    eSmaModbusRegister_meterForBatteryChargingAmpereHours = 30567U,
    /** U32 FIX0 */
    eSmaModbusRegister_meterForBatteryDischargingInAmpereHours = 30569U,
    /** U32 FIX0 */
    eSmaModbusRegister_meterReadingConsumptionMeter = 30571U,
    /** U32 Duration */
    eSmaModbusRegister_operatingTimeOfGenerator = 30573U,
    /** U32 FIX0 */
    eSmaModbusRegister_releasedEnergyFromGenerator = 30575U,
    /** U32 FIX0 */
    eSmaModbusRegister_purchasedElectricityToday = 30577U,
    /** U32 FIX0 */
    eSmaModbusRegister_feedInToday = 30579U,
    /** U32 FIX0 */
    eSmaModbusRegister_meterReadingPurchasedElectricityMeter = 30581U,
    /** U32 FIX0 */
    eSmaModbusRegister_meterReadingOfGridFeedInMeter = 30583U,
    /** U32 Duration */
    eSmaModbusRegister_powerOutageTime = 30585U,
    /** U32 FIX0 */
    eSmaModbusRegister_meterReadingPvProductionMeter = 30587U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalIncreasedSelfConsumption = 30589U,
    /** U32 FIX0 */
    eSmaModbusRegister_increasedSelfConsumptionToday = 30591U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalEnergyconsumedInternally = 30593U,
    /** U32 FIX0 */
    eSmaModbusRegister_consumedEnergy = 30595U,
    /** U32 FIX0 */
    eSmaModbusRegister_fedEnergy = 30597U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfGridConnections = 30599U,
    /** U32 Duration */
    eSmaModbusRegister_operatingTimeOfInteriorFan3 = 30601U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput1_S32 = 30769U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput1_S32 = 30771U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput1_S32 = 30773U,
    /** S32 FIX0 */
    eSmaModbusRegister_activePowerOnallLineConductors = 30775U,
    /** S32 FIX0 */
    eSmaModbusRegister_activePowerOfLineConductorL1 = 30777U,
    /** S32 FIX0 */
    eSmaModbusRegister_activePowerOfLineConductorL2 = 30779U,
    /** S32 FIX0 */
    eSmaModbusRegister_activePowerOfLineConductorL3 = 30779U,
    /** U32 FIX2 */
    eSmaModbusRegister_lineVoltageLineConductorL1ToN = 30783U,
    /** U32 FIX2 */
    eSmaModbusRegister_lineVoltageLineConductorL2ToN = 30785U,
    /** U32 FIX2 */
    eSmaModbusRegister_lineVoltageLineConductorL3ToN = 30787U,
    /** U32 FIX2 */
    eSmaModbusRegister_lineVoltageLineConductorL1ToL2 = 30789U,
    /** U32 FIX2 */
    eSmaModbusRegister_lineVoltageLineConductorL2ToL3 = 30791U,
    /** U32 FIX2 */
    eSmaModbusRegister_lineVoltageLineConductorL3ToL1 = 30793U,
    /** U32 FIX3 */
    eSmaModbusRegister_lineCurrentOnAllLineConductors = 30795U,
    /** U32 FIX3 */
    eSmaModbusRegister_lineCurrentOfLineConductorL1 = 30797U,
    /** U32 FIX3 */
    eSmaModbusRegister_lineCurrentOfLineConductorL2 = 30799U,
    /** U32 FIX3 */
    eSmaModbusRegister_lineCurrentOfLineConductorL3 = 30801U,
    /** U32 FIX2 */
    eSmaModbusRegister_powerFrequency = 30803U,
    /** S32 FIX0 */
    eSmaModbusRegister_reactivePowerOnAllLineConductors = 30805U,
    /** S32 FIX0 */
    eSmaModbusRegister_reactivePowerOfLineConductorL1 = 30807U,
    /** S32 FIX0 */
    eSmaModbusRegister_reactivePowerOfLineConductorL2 = 30809U,
    /** S32 FIX0 */
    eSmaModbusRegister_reactivePowerOfLineConductorL3 = 30811U,
    /** S32 FIX0 */
    eSmaModbusRegister_apparentPowerOnAllLineConductors = 30813U,
    /** S32 FIX0 */
    eSmaModbusRegister_apparentPowerOnLineConductorL1 = 30815U,
    /** S32 FIX0 */
    eSmaModbusRegister_apparentPowerOnLineConductorL2 = 30817U,
    /** S32 FIX0 */
    eSmaModbusRegister_apparentPowerOnLineConductorL3 = 30819U,
    /** U32 FIX2 */
    eSmaModbusRegister_totalDisplacementPowerFactorOfallLineConductors = 30821U,
    /** U32 See \p smaModbus_excitationTypeOfCosPhi_e */
    eSmaModbusRegister_excitationTypeOfCosPhi = 30823U,
    /** S32 FIX3 */
    eSmaModbusRegister_batteryCurrent = 30843U,
    /** U32 FIX0 */
    eSmaModbusRegister_currentBatteryStateOfCharge = 30845U,
    /** U32 FIX0 */
    eSmaModbusRegister_currentBatteryCapacity = 30847U,
    /** S32 TEMP */
    eSmaModbusRegister_batteryTemperature = 30849U,
    /** U32 FIX2 */
    eSmaModbusRegister_batteryVoltage = 30851U,
    /** U32 See \p smaModbus_activeBatteryChargingMode_e */
    eSmaModbusRegister_activeBatteryChargingMode = 20853U,
    /** U32 FIX2 */
    eSmaModbusRegister_currentBatteryChargingVoltageSetpoint = 30855U,
    /** S32 FIX0 */
    eSmaModbusRegister_numberOfBatteryChargeThroughputs = 30857U,
    /** U32 See \p smaModbus_batteryMaintenanceChargeStatus_e */
    eSmaModbusRegister_batteryMaintenanceChargeStatus = 30859U,
    /** S32 FIX0 */
    eSmaModbusRegister_loadPower = 30861U,
    /** U32 FIX0 */
    eSmaModbusRegister_currentPvArrayPower = 30863U,
    /** S32 FIX0 */
    eSmaModbusRegister_powerPurchasedElectricity = 30865U,
    /** S32 FIX0 */
    eSmaModbusRegister_powerGridFeedIn = 30867U,
    /** S32 FIX0 */
    eSmaModbusRegister_powerPvGeneration = 30869U,
    /** U32 FIX0 */
    eSmaModbusRegister_currentSelfConsumption = 30871U,
    /** S32 FIX0 */
    eSmaModbusRegister_currentIncreasedSelfConsumption = 30873U,
    /** U32 See \p smaModbus_multifunctionRelayStatus_e */
    eSmaModbusRegister_multifunctionRelayStatus = 30875U,
    /** U32 See \p smaModbus_electricitySupplyStatus_e */
    eSmaModbusRegister_electricitySupplyStatus = 30877U,
    /** U32 See \p smaModbus_reasonForRequestingGenerator_e */
    eSmaModbusRegister_reasonForRequestingGenerator = 30879U,
    /** U32 See \p smaModbus_pvSystemUtilityGridConnection_e */
    eSmaModbusRegister_pvSystemUtilityGridConnection = 30881U,
    /** U32 See \p smaModbus_statusOfUtilityGrid_e */
    eSmaModbusRegister_statusOfUtilityGrid = 30883U,

    /**
     * @todo Too lazy to fill in the remaining as I'll never be using these.
     * Please do feel free to update this yourself.
     * I stopped at page 27 of 73 (Technical Information - SMA Modbus Interface for SUNNY BOy / SUNNY TRIPOWER)
     */

    /** U32 See \p smaModbus_dataTransferRateOfNetworkTerminal_e */
    eSmaModbusRegister_dataTransferRateOfNetworkTerminalA = 30925U,
    /** U32 See \p smaModbus_duplexModeOfNetworkTerminal_e */
    eSmaModbusRegister_duplexModeOfNetworkTerminalA = 30927U,
    /** U32 See \p smaModbus_speedwireConnectionStatusOfNetworkTerminal_e */
    eSmaModbusRegister_speedwireConnectionStatusOfNetworkTerminalA = 30929U,
    /** U32 See \p smaModbus_dataTransferRateOfNetworkTerminal_e */
    eSmaModbusRegister_dataTransferRateOfNetworkTerminalB = 30931U,
    /** U32 See \p smaModbus_duplexModeOfNetworkTerminal_e */
    eSmaModbusRegister_duplexModeOfNetworkTerminalB = 30933U,
    /** U32 See \p smaModbus_speedwireConnectionStatusOfNetworkTerminal_e */
    eSmaModbusRegister_speedwireConnectionStatusOfNetworkTerminalB = 30935U,
    /** U32 See \p smaModbus_dataTransferRateOfNetworkTerminal_e */
    eSmaModbusRegister_dataTransferRateOfNetworkTerminalC = 30937U,
    /** U32 See \p smaModbus_duplexModeOfNetworkTerminal_e */
    eSmaModbusRegister_duplexModeOfNetworkTerminalC = 30939U,
    /** U32 See \p smaModbus_speedwireConnectionStatusOfNetworkTerminal_e */
    eSmaModbusRegister_speedwireConnectionStatusOfNetworkTerminalC = 30941U,
    /** U32 See \p smaModbus_dataTransferRateOfNetworkTerminal_e */
    eSmaModbusRegister_dataTransferRateOfNetworkTerminalD = 30943U,
    /** U32 See \p smaModbus_duplexModeOfNetworkTerminal_e */
    eSmaModbusRegister_duplexModeOfNetworkTerminalD = 30945U,
    /** U32 See \p smaModbus_speedwireConnectionStatusOfNetworkTerminal_e */
    eSmaModbusRegister_speedwireConnectionStatusOfNetworkTerminalD = 30947U,
    /** U32 FIX3 */
    eSmaModbusRegister_displacementPowerFactor = 30949U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerWithoutBattery = 30951U,
    /** S32 TEMP */
    eSmaModbusRegister_internalTemperature = 30953U,
    /** U32 See \p smaModbus_operatingStatusOfBattery_e */
    eSmaModbusRegister_operatingStatusOfBattery = 30955U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput2_S32 = 30957U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput2_S32 = 30959U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput2_S32 = 30961U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput3_S32 = 30963U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput3_S32 = 30965U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput3_S32 = 30967U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput4_S32 = 30969U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput4_S32 = 30971U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput4_S32 = 30973U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcLinkVoltage = 30975U,
    /** S32 FIX3 */
    eSmaModbusRegister_lineCurrentOfLineConductorL1_2 = 30977U,
    /** S32 FIX3 */
    eSmaModbusRegister_lineCurrentOfLineConductorL2_2 = 30979U,
    /** S32 FIX3 */
    eSmaModbusRegister_lineCurrentOfLineConductorL3_2 = 30981U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvPower = 30983U,
    /** S32 FIX3 */
    eSmaModbusRegister_totalCurrentAtTheExternalGridConnection = 30985U,

    /**
     * @brief Page 30/73
     */

    /** STR32 UTF8 */
    eSmaModbusRegister_currentSpeedwireIpAddress = 31017U,
    /** STR32 UTF8 */
    eSmaModbusRegister_currentSpeedwireSubnetMask = 31025U,
    /** STR32 UTF8 */
    eSmaModbusRegister_currentSpeedwireGatewayAddress = 31033U,
    /** STR32 UTF8 */
    eSmaModbusRegister_currentSpeedwireDnsServerAddress = 31041U,

    /**
     * @brief Page 31/73
     *
     */
    /** U32 FIX0 */
    eSmaModbusRegister_totalPvEnergy = 31063U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalPvEnergyToday = 31065U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfEqualizationChargesOfTheBattery = 31067U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfFullChargesOfTheBattery = 31069U,
    /** U32 FIX0 */
    eSmaModbusRegister_relativeBatteryDischargingSinceTheLastFullCharge = 31071U,
    /** U32 FIX0 */
    eSmaModbusRegister_relativeBatteryDischargingSinceTheLastEqualizationCharge = 31073U,
    /** U32 Duration */
    eSmaModbusRegister_energyMeterRunTime = 31075U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyAtSolarChargeController1 = 31077U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyAtSolarChargeController2 = 31079U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyAtSolarChargeController3 = 31081U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyAtSolarChargeController4 = 31083U,
    /** U32 FIX0 */
    eSmaModbusRegister_nominalPowerInOkMode = 31085U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyProductedToday = 31091U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyProductedYesterday = 31093U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyProducedCurrentMonth = 31095U,
    /** U32 FIX0 */
    eSmaModbusRegister_pvEnergyProducedPreviousMonth = 31097U,
    /** U32 FIX0 */
    eSmaModbusRegister_generatorEnergyProducedToday = 31099U,
    /** U32 FIX0 */
    eSmaModbusRegister_generatorEnergyProducedYesterday = 31101U,
    /** U32 FIX0 */
    eSmaModbusRegister_generatorEnergyProducedCurrentMonth = 31103U,
    /** U32 FIX0 */
    eSmaModbusRegister_generatorEnergyProducedPreviousMonth = 31105U,
    /** U32 FIX0 */
    eSmaModbusRegister_gridFeedInToday = 31107U,
    /** U32 FIX0 */
    eSmaModbusRegister_energyFedIntoTheUtilityGridYesterday = 31109U,
    /** U32 FIX0 */
    eSmaModbusRegister_energyFedIntoTheUtilityGridCurrentMonth = 31111U,
    /** U32 FIX0 */
    eSmaModbusRegister_energyFedIntoTheUtilityGridPreviousMonth = 31119U,
    /** U32 FIX0 */
    eSmaModbusRegister_consumedEnergyToday = 31121U,
    /** U32 FIX0 */
    eSmaModbusRegister_consumedEnergyYesterday = 31123U,
    /** U32 FIX0 */
    eSmaModbusRegister_consumedEnergyCurrentMonth = 31125U,
    /** U32 FIX0 */
    eSmaModbusRegister_consumedEnergyPreviousMonth = 31127U,
    /** U32 FIX0 */
    eSmaModbusRegister_unusedPvPower = 31129U,
    /** S32 FIX0 */
    eSmaModbusRegister_availablePvPower = 31131U,
    /** S32 FIX0 */
    eSmaModbusRegister_internalPvPowerLimitation = 31133U,
    /** S32 FIX0 */
    eSmaModbusRegister_reactivePowerOfTheLoad = 31135U,
    /** S32 FIX0 */
    eSmaModbusRegister_maximumShortTermPowerReduction = 31137U,
    /** S32 FIX0 */
    eSmaModbusRegister_currentPvReactivePowerFedIn = 31139U,
    /** S32 FIX0 */
    eSmaModbusRegister_currentPvApparentPowerFedIn = 31141U,
    /** S32 FIX0 */
    eSmaModbusRegister_monitoringValueReturn = 31143U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput1 = 31281U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput1 = 31283U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput1 = 31285U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput2 = 31287U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput2 = 31289U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput2 = 31291U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput3 = 31293U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput3 = 31295U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput3 = 31297U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput4 = 31299U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput4 = 31301U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput4 = 31303U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput5 = 31305U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput5 = 31307U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput5 = 31309U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput6 = 31311U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput6 = 31313U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput6 = 31315U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput7 = 31317U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput7 = 31319U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput7 = 31321U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput8 = 31323U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput8 = 31325U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput8 = 31327U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput9 = 31329U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput9 = 31331U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput9 = 31333U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput10 = 31335U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput10 = 31337U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput10 = 31339U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput11 = 31341U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput11 = 31343U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput11 = 31345U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput12 = 31347U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput12 = 312349,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput12 = 31351U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput13 = 31353U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput13 = 31355U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput13 = 31357U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput14 = 31359U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput14 = 31361U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput14 = 31363U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput15 = 31365U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput15 = 31367U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput15 = 31369U,
    /** S32 FIX2 */
    eSmaModbusRegister_dcVoltageInput16 = 31371U,
    /** S32 FIX3 */
    eSmaModbusRegister_dcCurrentInput16 = 31373U,
    /** S32 FIX0 */
    eSmaModbusRegister_dcPowerInput16 = 31375U,
    /** U32 FIX0 */
    eSmaModbusRegister_numberOfDcCurrentMeasurementUnits = 31791U,
    /** S32 FIX3 */
    eSmaModbusRegister_stringCurrentOfString1 = 31793U,

    /**
     * @brief Page 33/73
     */

    /** S32 FIX3 */
    eSmaModbusRegister_stringCurrentOfString128 = 32047U,

    /**
     * @brief Page 34/73
     */
    /** U64 Duration */
    eSmaModbusRegister_operatingTimeOfInteriorFan1 = 34097U,
    /** U64 Duration */
    eSmaModbusRegister_operatingTimeOfInteriorFan2 = 34101U,
    /** U64 Duration */
    eSmaModbusRegister_operatingTimeOfHeatsinkFan = 34105U,
    /** S32 TEMP */
    eSmaModbusRegister_heatSinkTemperature1 = 34109U,
    /** S32 TEMP */
    eSmaModbusRegister_interiorTemperature1 = 34113U,
    /** S32 TEMP */
    eSmaModbusRegister_transformerTemperature1 = 34121U,
    /** S32 TEMP */
    eSmaModbusRegister_externalTemperature1OfSupplyAir = 34125U,
    /** S32 TEMP */
    eSmaModbusRegister_highestMeasuredExternalTemperature1 = 34127U,
    /** S32 TEMP */
    eSmaModbusRegister_ambientTemperature = 34609U,
    /** S32 TEMP */
    eSmaModbusRegister_highestMeasuredAmbientTemperature = 34611U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalIrradiationOnTheSensorSurface = 34613U,
    /** U32 FIX1 */
    eSmaModbusRegister_windSpeed = 34615U,
    /** U32 FIX2 */
    eSmaModbusRegister_humidity = 34617U,
    /** U32 FIX2 */
    eSmaModbusRegister_airPressure = 34619U,
    /** S32 TEMP */
    eSmaModbusRegister_pvModuleTemperature = 34621U,
    /** U32 FIX0 */
    eSmaModbusRegister_totalIrradiationOnTheExternalIrradiationSensor = 34623U,
    /** S32 TEMP */
    eSmaModbusRegister_ambientTemperatureF = 34625U,
    /** S32 TEMP */
    eSmaModbusRegister_ambientTemperatureK = 34627U,
    /** S32 TEMP */
    eSmaModbusRegister_pvModuleTemperatureF = 34629U,
    /** S32 TEMP */
    eSmaModbusRegister_pvModuleTemperatureK = 34631U,

    /**
     * @brief Page 35/72
     */

    /** U64 FIX0 */
    eSmaModbusRegister_numberOfEventsForUser = 35377U,
    /** U64 FIX0 */
    eSmaModbusRegister_numberOfEventsForInstaller = 35381U,
    /** U64 FIX0 */
    eSmaModbusRegister_numberOfEventsForService = 35385U,

    /**
     * @brief Page 36/73
     * @warning The following registers are marked RW
     */
} smaModbus_registers_e;

/** @brief Device class */
typedef enum smaModbus_deviceClass {
    eSmaModbusDeviceClass_allDevices = 8000U,
    eSmaModbusDeviceClass_pvInverter = 8001U,
    eSmaModbusDeviceClass_windPowerInverter = 8002U,
    eSmaModbusDeviceClass_batteryInverter = 8007U,
    eSmaModbusDeviceClass_load = 8033U,
    eSmaModbusDeviceClass_sensorTechnologyGeneral = 8064U,
    eSmaModbusDeviceClass_energyMeter = 8065U,
    eSmaModbusDeviceClass_communicationProducts = 8128U,
} smaModbus_deviceClass_e;

/** @brief Manufacturer specification */
typedef enum smaModbus_manufacturerSpecification {
    eSmaModbusManufacturerSpecification_sma = 461U,
} smaModbus_manufacturerSpecification_e;

/** @brief Status of the device */
typedef enum smaModbus_statusOfTheDevice {
    eSmaModbusStatusOfTheDevice_fault = 35U,
    eSmaModbusStatusOfTheDevice_off = 303U,
    eSmaModbusStatusOfTheDevice_ok = 307U,
    eSmaModbusStatusOfTheDevice_warning = 455U,
} smaModbus_statusOfTheDevice_e;

/** @brief Recommended action */
typedef enum smaModbus_recommendedAction {
    eSmaModbusRecommendedAction_contactManufacturer = 336U,
    eSmaModbusRecommendedAction_contactInstaller = 337U,
    eSmaModbusRecommendedAction_invalid = 338U,
    eSmaModbusRecommendedAction_noRecommendedAction = 887U,
} smaModbus_recommendedAction_e;

/** @brief Status message */
typedef enum smaModbus_statusMessage {
    eSmaModbusStatusMessage_noMessage = 886U,
} smaModbus_statusMessage_e;

/** @brief Status description */
typedef enum smaModbus_statusDescription {
    eSmaModbusStatusDescription_noDescription = 885U,
} smaModbus_statusDescription_e;

/** @brief Utility grid contactor */
typedef enum smaModbus_utilityGridContactor {
    eSmaModbusUtilityGridContactor_closed = 51U,
    eSmaModbusUtilityGridContactor_open = 311U,
} smaModbus_utilityGridContactor_e;

/** @brief Temperature derating */
typedef enum smaModbus_temperatureDerating {
    eSmaModbusTemperatureDerating_noDerating = 302U,
    eSmaModbusTemperatureDerating_temperatureDerating = 557U,
    eSmaModbusTemperatureDerating_notActive = 884U,
    eSmaModbusTemperatureDerating_wmaxDerating = 1704U,
    eSmaModbusTemperatureDerating_frequencyDerating = 1705U,
    eSmaModbusTemperatureDerating_deratingDueToPvCurrentLimitation = 1706U,
} smaModbus_temperatureDerating_e;

/** @brief Status of key switch */
typedef enum smaModbus_statusOfKeySwitch {
    eSmaModbusStatusOfKeySwitch_off = 381U,
    eSmaModbusStatusOfKeySwitch_on = 569U,
} smaModbus_statusOfKeySwitch_e;

/** @brief Backup mode status */
typedef enum smaModbus_backupModeStatus {
    eSmaModbusBackupModeStatus_gridOperation = 1440U,
    eSmaModbusBackupModeStatus_standAloneMode = 1441U
} smaModbus_backupModeStatus_e;

/** @brief Grid type */
typedef enum smaModbus_gridType {
    eSmaModbusGridType_277V = 1433U,
    eSmaModbusGridType_208V = 1434U,
    eSmaModbusGridType_240V = 1435U,
    eSmaModbusGridType_208VwoNeutralConductor = 1436U,
    eSmaModbusGridType_240VwoNeutralConductor = 1437U
} smaModbus_gridType_e;

/** @brief Status of the GFDI relay */
typedef enum smaModbus_statusOfTheGfdiRelay {
    eSmaModbusStatusOfTheGfdiRelay_closed = 51U,
    eSmaModbusStatusOfTheGfdiRelay_open = 311U,
} smaModbus_statusOfTheGfdiRelay_e;

/** @brief Status of current restart interlock */
typedef enum smaModbus_statusOfCurrentRestartInterlock {
    eSmaModbusStatusOfCurrentRestartInterlock_frequencyNotPermitted = 257U,
    eSmaModbusStatusOfCurrentRestartInterlock_fastShutDown = 1690U,
    eSmaModbusStatusOfCurrentRestartInterlock_overvoltage = 2386U,
    eSmaModbusStatusOfCurrentRestartInterlock_undervoltage = 2387U,
    eSmaModbusStatusOfCurrentRestartInterlock_overfrequency = 2388U,
    eSmaModbusStatusOfCurrentRestartInterlock_underfrequency = 2389U,
    eSmaModbusStatusOfCurrentRestartInterlock_passiveIslandingDetection = 2390U,
    eSmaModbusStatusOfCurrentRestartInterlock_phaseLostDetection = 2490U,
    eSmaModbusStatusOfCurrentRestartInterlock_phaseLockedLoopError = 3165U,
    eSmaModbusStatusOfCurrentRestartInterlock_phaseLostDetectionOnLowVoltageSide = 3166U,
    eSmaModbusStatusOfCurrentRestartInterlock_activeIslandingDetection = 3167U,
} smaModbus_statusOfCurrentRestartInterlock_e;

/** @brief State of DC switch */
typedef enum smaModbus_stateOfDcSwitch {
    eSmaModbusStateOfDcSwitch_closed = 51U,
    eSmaModbusStateOfDcSwitch_open = 311U,
} smaModbus_stateOfDcSwitch_e;

/** @brief DC switch 1 to 32 */
typedef enum smaModbus_dcSwitch {
    eSmaModbusDcSwitch_closed = 51U,
    eSmaModbusDcSwitch_open = 331U,
} smaModbus_dcSwitch_e;

/** @brief Error message DC switch 1 to 32 */
typedef enum smaModbus_errorMessageDcSwitch {
    eSmaModbusErrorMessageDcSwitch_90PercentOfTheDcSwitchCyclesReached = 1508U,
    eSmaModbusErrorMessageDcSwitch_100PercentOfTheDcSwitchCyclesReached = 1509U,
    eSmaModbusErrorMessageDcSwitch_dcSwitchHasTripped = 1694U,
    eSmaModbusErrorMessageDcSwitch_dcSwitchWaitingForConnection = 1695U,
    eSmaModbusErrorMessageDcSwitch_dcSwitchBlockedBySpindle = 1696U,
    eSmaModbusErrorMessageDcSwitch_dcSwitchManuallyBlocked = 1697U,
    eSmaModbusErrorMessageDcSwitch_dcSwitchTrippedThreeTimes = 1698U,
    eSmaModbusErrorMessageDcSwitch_dcSwitchIsDefective = 1699U,
} smaModbus_errorMessageDcSwitch_e;

/** @brief Excitation type of cos phi */
typedef enum smaModbus_excitationTypeOfCosPhi {
    eSmaModbusExcitationTypeOfCosPhi_leading = 1041U,
    eSmaModbusExcitationTypeOfCosPhi_lagging = 1042U,
} smaModbus_excitationTypeOfCosPhi_e;

/** @brief Active battery charging mode */
typedef enum smaModbus_activeBatteryChargingMode {
    eSmaModbusActiveBatteryChargingMode_boostCharge = 1767U,
    eSmaModbusActiveBatteryChargingMode_fullCharge = 1768U,
    eSmaModbusActiveBatteryChargingMode_equalizationCharge = 1769U,
    eSmaModbusActiveBatteryChargingMode_floatCharge = 1770U,
} smaModbus_activeBatteryChargingMode_e;

/** @brief Battery maintenance charge status */
typedef enum smaModbus_batteryMaintenanceChargeStatus {
    eSmaModbusBatteryMaintenanceChargeStatus_inactive = 803U,
    eSmaModbusBatteryMaintenanceChargeStatus_chargeWithSolarPower = 1771U,
    eSmaModbusBatteryMaintenanceChargeStatus_chargeWithSolarAndGridPower = 1772U,
} smaModbus_batteryMaintenanceChargeStatus_e;

/** @brief Multifunction relay status */
typedef enum smaModbus_multifunctionRelayStatus {
    eSmaModbusMultifunctionRelayStatus_closed = 51U,
    eSmaModbusMultifunctionRelayStatus_open = 311U,
} smaModbus_multifunctionRelayStatus_e;

/** @brief Electricity supply status */
typedef enum smaModbus_electricitySupplyStatus {
    eSmaModbusElectricitySupplyStatus_off = 303U,
    eSmaModbusElectricitySupplyStatus_utilityGridConnected = 1461U,
    eSmaModbusElectricitySupplyStatus_backupNotAvailable = 1462U,
    eSmaModbusElectricitySupplyStatus_backup = 1463U,
} smaModbus_electricitySupplyStatus_e;

/** @brief Reason for requesting generator */
typedef enum smaModbus_reasonForRequestingGenerator {
    eSmaModbusReasonForRequestingGenerator_noRequest = 1773U,
    eSmaModbusReasonForRequestingGenerator_load = 1774U,
    eSmaModbusReasonForRequestingGenerator_timeControl = 1775U,
    eSmaModbusReasonForRequestingGenerator_manualOnehour = 1776U,
    eSmaModbusReasonForRequestingGenerator_manualStart = 1777U,
    eSmaModbusReasonForRequestingGenerator_externalSource = 1778U,
} smaModbus_reasonForRequestingGenerator_e;

/** @brief PV system utility grid connection */
typedef enum smaModbus_pvSystemUtilityGridConnection {
    eSmaModbusPvSystemUtilityGridConnection_disconnected = 1779U,
    eSmaModbusPvSystemUtilityGridConnection_utilityGrid = 1780U,
    eSmaModbusPvSystemUtilityGridConnection_standAloneGrid = 1781U,
} smaModbus_pvSystemUtilityGridConnection_e;

/** @brief Status of utility grid */
typedef enum smaModbus_statusOfUtilityGrid {
    eSmaModbusStatusOfUtilityGrid_off = 303U,
    eSmaModbusStatusOfUtilityGrid_waitingForValidAcUtilityGrid = 1394U,
    eSmaModbusStatusOfUtilityGrid_utilityGridConnected = 1461U,
    eSmaModbusStatusOfUtilityGrid_waiting = 1466U,
    eSmaModbusStatusOfUtilityGrid_initialization = 1787U,
    eSmaModbusStatusOfUtilityGrid_gridOperationWithoutFeedback = 2183U,
    eSmaModbusStatusOfUtilityGrid_energySavngInTheUtilityGrid = 2184U,
    eSmaModbusStatusOfUtilityGrid_endEnergySavingInTheUtilityGrid = 2185U,
    eSmaModbusStatusOfUtilityGrid_startEnergySavingInTheUtilityGrid = 2186U,
} smaModbus_statusOfUtilityGrid_e;

/** @brief Data transfer of network terminal */
typedef enum smaModbus_dataTransferRateOfNetworkTerminal {
    eSmaModbusDataTransferOfNetworkTerminal_10Mbit = 1720U,
    eSmaModbusDataTransferOfNetworkTerminal_100Mbit = 1721U,
    eSmaModbusDataTransferOfNetworkTerminal_notConnected = 1725U,
} smaModbus_dataTransferRateOfNetworkTerminal_e;

/** @brief Duplex mode of network terminal */
typedef enum smaModbus_duplexModeOfNetworkTerminal {
    eSmaModbusDuplexModeOfNetworkTerminal_notConnected = 1725U,
    eSmaModbusDuplexModeOfNetworkTerminal_halfDuplex = 1726U,
    eSmaModbusDuplexModeOfNetworkTerminal_fullDuplex = 1727U,
} smaModbus_duplexModeOfNetworkTerminal_e;

/** @brief Speedwire connection status of network terminal */
typedef enum smaModbus_speedwireConnectionStatusOfNetworkTerminal {
    eSmaModbusSpeedwireConnectionStatusOfNetworkTerminal_alarm = 35U,
    eSmaModbusSpeedwireConnectionStatusOfNetworkTerminal_ok = 307U,
    eSmaModbusSpeedwireConnectionStatusOfNetworkTerminal_warning = 455U,
    eSmaModbusSpeedwireConnectionStatusOfNetworkTerminal_notConnected = 1725U,
} smaModbus_speedwireConnectionStatusOfNetworkTerminal_e;

/** @brief Operating status of battery */
typedef enum smaModbus_operatingStatusOfBattery {
    eSmaModbusOperatingStatusOfBattery_off = 303U,
    eSmaModbusOperatingStatusOfBattery_batteryStandby = 2291U,
    eSmaModbusOperatingStatusOfBattery_batteryCharging = 2292U,
    eSmaModbusOperatingStatusOfBattery_batteryDischarging = 2293U,
} smaModbus_operatingStatusOfBattery_e;

#ifdef __cplusplus
} /* extern "C" {*/
#endif

#endif