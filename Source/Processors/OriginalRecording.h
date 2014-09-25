/*
 ------------------------------------------------------------------

 This file is part of the Open Ephys GUI
 Copyright (C) 2013 Florian Franzen

 ------------------------------------------------------------------

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#ifndef ORIGINALRECORDING_H_INCLUDED
#define ORIGINALRECORDING_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

#include "RecordEngine.h"
#include <stdio.h>

#define HEADER_SIZE 1024
#define BLOCK_LENGTH 1024

class OriginalRecording : public RecordEngine
{
public:
    OriginalRecording();
    ~OriginalRecording();

    void openFiles(File rootFolder, int experimentNumber, int recordingNumber);
    void closeFiles();
    void writeData(AudioSampleBuffer& buffer, int nSamples);
    void writeEvent(int eventType, MidiMessage& event, int samplePosition);
    void addChannel(int index, Channel* chan);
    void resetChannels();
    void updateTimeStamp(int64 timestamp);
    void addSpikeElectrode(int index, SpikeRecordInfo* elec);
    void writeSpike(const SpikeObject& spike, int electrodeIndex);

private:
    String getFileName(Channel* ch);
    void openFile(File rootFolder, Channel* ch);
    String generateHeader(Channel* ch);
    void writeContinuousBuffer(const float* data, int nSamples, int channel);
    void writeTimestampAndSampleCount(FILE* file);
    void writeRecordMarker(FILE* file);

    void openSpikeFile(File rootFolder, SpikeRecordInfo* elec);
    String generateSpikeHeader(SpikeRecordInfo* elec);

	void openMessageFile(File rootFolder);
	void writeTTLEvent(MidiMessage& event, int samplePosition);
	void writeMessage(MidiMessage& event, int samplePosition);

    bool separateFiles;
    int blockIndex;
    int recordingNumber;
    int experimentNumber;

    /** Holds data that has been converted from float to int16 before
        saving.
    */
    int16* continuousDataIntegerBuffer;

    /** Holds data that has been converted from float to int16 before
        saving.
    */
    float* continuousDataFloatBuffer;

    /** Used to indicate the end of each record */
    char* recordMarker;

    AudioSampleBuffer zeroBuffer;
    int64 timestamp;

    FILE* eventFile;
	FILE* messageFile;
    Array<FILE*> fileArray;
    Array<FILE*> spikeFileArray;
    CriticalSection diskWriteLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OriginalRecording);
};

#endif  // ORIGINALRECORDING_H_INCLUDED
