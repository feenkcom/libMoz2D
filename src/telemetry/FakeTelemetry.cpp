/*
 * FakeTelemetry.cpp
 *
 *  Created on: Jul 28, 2016
 *      Author: aliaksei
 */

#include "mozilla/Telemetry.h"

namespace mozilla {
namespace Telemetry {

void CreateStatisticsRecorder(){}
void DestroyStatisticsRecorder(){}
void Init(){}
void Accumulate(ID id, uint32_t sample){}
void Accumulate(ID id, const nsCString& key, uint32_t sample){}
void Accumulate(const char* name, uint32_t sample){}
void Accumulate(const char *name, const nsCString& key, uint32_t sample){}
void AccumulateCategorical(ID id, const nsCString& label){}
void AccumulateTimeDelta(ID id, TimeStamp start, TimeStamp end){}
void ClearHistogram(ID id){}
void SetHistogramRecordingEnabled(ID id, bool enabled){}

}
}
