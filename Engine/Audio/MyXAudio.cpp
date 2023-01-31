#include "MyXAudio.h"
#include <fstream>
#include <cassert>

MyXAudio::MyXAudio()
{
	handle = 0;
	soundData.resize(handle);

	HRESULT result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

MyXAudio::~MyXAudio()
{
	xAudio2.Reset();
	for (size_t i = 0; i < soundData.size(); i++)
	{
		SoundUnload(&soundData[i]);
	}
}

MyXAudio* MyXAudio::GetInstance()
{
	static MyXAudio* instance = new MyXAudio;
	return instance;
}

void MyXAudio::DeleteInstance()
{
	delete MyXAudio::GetInstance();
}

int MyXAudio::SoundLoadWave(const char* filename)
{
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunk format = {};

	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	if (strncmp(data.id, "JUNK", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) { assert(0); }
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();

	SoundData _soundData = {};

	_soundData.wfex = format.fmt;
	_soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	_soundData.bufferSize = data.size;

	soundData.push_back(_soundData);

	return handle++;
}

void MyXAudio::SoundUnload(SoundData* soundData_)
{
	delete[] soundData_->pBuffer;

	soundData_->pBuffer = 0;
	soundData_->bufferSize = 0;
	soundData_->wfex = {};
}

void MyXAudio::SoundPlayWave(IXAudio2* xAudio2_, const SoundData& soundData_)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData_.wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData_.pBuffer;
	buf.AudioBytes = soundData_.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}

void MyXAudio::SoundPlayWave(int handle_)
{
	HRESULT result;

	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData[handle_].wfex);
	assert(SUCCEEDED(result));

	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData[handle_].pBuffer;
	buf.AudioBytes = soundData[handle_].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}
