#pragma once

namespace sound {

	/* サウンドデータのスーパークラス */
	class AudioData {
		/* 未実装 */
	public:
		virtual ~AudioData() {};
		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual void setVolume(double volume) = 0;
	};

}
