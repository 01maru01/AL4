#pragma once
#include <xaudio2.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib,"xaudio2.lib")

struct ChunkHeader {
	char id[4];
	int32_t size;
};
struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};
struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};
struct SoundData {
	WAVEFORMATEX wfex;
	BYTE* pBuffer;
	unsigned int bufferSize;
};
class MyXAudio
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	IXAudio2MasteringVoice* masterVoice;
	std::vector<SoundData> soundData;
	int handle;
	void SoundUnload(SoundData* soundData_);

	MyXAudio();
	~MyXAudio();
public:
	ComPtr<IXAudio2> xAudio2;

	static MyXAudio* GetInstance();
	static void DeleteInstance();
	MyXAudio(const MyXAudio& obj) = delete;
	MyXAudio& operator=(const MyXAudio& obj) = delete;

	int SoundLoadWave(const char* filename);
	void SoundPlayWave(IXAudio2* xAudio2_, const SoundData& soundData_);
	void SoundPlayWave(int handle_);
};

