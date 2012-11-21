#include "snddata.h"
#include <fstream>

/* 4�o�C�g�P�ʂ̃`�����N�̃`�F�b�N */
bool sound::WaveData::checkTag(std::ifstream& file, const std::string& tag) {
	char chkdat[8] = {0,};
	file.read(chkdat, 4);
	return std::string(chkdat).compare(tag) == 0;
}

/* �t�H�[�}�b�g�ǂݍ��� */
void sound::WaveData::GetFormat(std::ifstream& file) {
	static_assert(sizeof(std::uint32_t) == 4, "sizeof(uint32_t) is not 4");
	static_assert(sizeof(std::uint16_t) == 2, "sizeof(uint16_t) is not 2");
	if (!checkTag(file, "WAVE")) throw "WAVE�`�����N���Ȃ��ł�";
	if (!checkTag(file, "fmt ")) throw "fmt �`�����N���Ȃ��ł�";
	// �w�b�_�T�C�Y
	std::uint32_t format_size; file.read(reinterpret_cast<char*>(&format_size), 4);
	// �f�[�^�t�H�[�}�b�g
	std::uint16_t id; file.read(reinterpret_cast<char*>(&id), 2);
	if (id == 1) format.wFormatTag = WAVE_FORMAT_PCM;
	else if (id == 2) format.wFormatTag = WAVE_FORMAT_ADPCM;
	else throw "�Ή����Ă��Ȃ��t�H�[�}�b�g�ł�";
	// �w�b�_�ǂݍ���
	file.read(reinterpret_cast<char*>(&format.nChannels), sizeof(format.nChannels));
	file.read(reinterpret_cast<char*>(&format.nSamplesPerSec), sizeof(format.nSamplesPerSec));
	file.read(reinterpret_cast<char*>(&format.nAvgBytesPerSec), sizeof(format.nAvgBytesPerSec));
	file.read(reinterpret_cast<char*>(&format.nBlockAlign), sizeof(format.nBlockAlign));
	file.read(reinterpret_cast<char*>(&format.wBitsPerSample), sizeof(format.wBitsPerSample));
	// �g���w�b�_���͖���
	file.ignore(format_size - 16);
}

/* �g�`�f�[�^�ǂݍ��� */
void sound::WaveData::ReadWaveData(std::ifstream& file) {
	static_assert(sizeof(std::uint32_t) == 4, "sizeof(uint32_t) is not 4");
	if (!checkTag(file, "data")) throw "data�`�����N���Ȃ��ł�";
	std::uint32_t size; file.read(reinterpret_cast<char*>(&size), 4);
	buffer.resize(size);
	// �ǂݍ���
	file.read(&buffer.front(), size);
}

/* WAVE�t�@�C���ǂݍ��� */
void sound::WaveData::Prepare(const std::string& filename) {
	std::memset(&format, 0, sizeof(format));
	std::memset(&buffer, 0, sizeof(buffer));
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file) throw "�t�@�C�����J���܂���ł���";
	if (!checkTag(file, "RIFF")) throw "RIFF�`�����N���Ȃ��ł�";
	file.ignore(4);
	GetFormat(file);
	ReadWaveData(file);
}

sound::WaveData::WaveData(IXAudio2** Engine, const std::string& filename, bool looped) {
	Prepare(filename);
	std::memset(&bufInfo, 0, sizeof(bufInfo));
	bufInfo.AudioBytes = buffer.size();
	bufInfo.pAudioData = reinterpret_cast<BYTE*>(&buffer[0]);
	bufInfo.LoopCount = (looped ? XAUDIO2_LOOP_INFINITE : 0);
	if (FAILED((*Engine)->CreateSourceVoice(&voice, &format)))
		throw "CreateSourceVoice���s�I�I";
	if (FAILED(voice->SubmitSourceBuffer(&bufInfo)))
		throw "SubmitSourceBuffer���s�I�I";
}

/* �Đ� */
void sound::WaveData::Play() {
	if (FAILED(voice->Start(0, XAUDIO2_COMMIT_NOW)))
		throw "Start���s�I�I";
}

/* ��~ */
void sound::WaveData::Stop() {
	if (FAILED(voice->Stop()))
		throw "Stop���s�I�I";
}

/* �f�X�g���N�^ */
sound::SoundData::~SoundData() {
	if (voice) {
		voice->Stop();
		voice->DestroyVoice();
	}
}
