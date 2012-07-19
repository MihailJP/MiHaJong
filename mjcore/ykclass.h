#ifndef YKCLASS_H
#define YKCLASS_H

#include <string>
#include <functional>
#include <set>
#include <cstdint>
#include "gametbl.h"

namespace yaku {
	namespace yakuCalculator {

		struct MENTSU_ANALYSIS;

		typedef std::function<bool
			(const GameTable* const, const MENTSU_ANALYSIS* const)> YAKUFUNC;

		enum hanUnit : uint8_t {Han, Mangan, Yakuman};
		class Yaku {
		public:
			class YAKU_HAN {
			public:
				class HAN { // 翻
				public:
					HAN() { han = 0; unit = Han; }
					HAN(int8_t h) { han = h; unit = Han; }
					HAN(int8_t h, hanUnit u) { han = h; unit = u; }
					int8_t getHan() {return this->han;}
					hanUnit getUnit() {return this->unit;}
				private:
					int8_t han; // 数値
					hanUnit unit; // 単位
				};
				HAN coreHan; // 縛りを満たす翻
				HAN bonusHan; // 縛りを満たさない翻
				YAKU_HAN() {coreHan = HAN(); bonusHan = HAN();}
				YAKU_HAN(HAN han) {coreHan = han; bonusHan = HAN();}
				YAKU_HAN(HAN han, HAN bonus) {coreHan = han; bonusHan = bonus;}
			};
		private:
			YAKU_HAN han;
			std::string yakuName; // 役の名前（文字列）
			YAKUFUNC yakuProc; // 役の判定方法
			std::set<std::string> suppressionList; // 下位役のリスト
			Yaku() {} // Default constructor
		public:
			bool checkYaku(const GameTable* const gameStat, const MENTSU_ANALYSIS* const mentsu) {
				 // 役を判定する
				return this->yakuProc(gameStat, mentsu);
			}
			std::string getName() {return this->yakuName;} // 役の名前を取得する
			YAKU_HAN getHan() {return han;}
			std::set<std::string> getSuppression() {return suppressionList;}
			// Constructor
#define YKCLASS_GENERAL_INIT \
	yakuName = std::string(name.begin(), name.end()); han = YAKU_HAN(cHan, bHan); yakuProc = YAKUFUNC(f); \
	suppressionList = std::set<std::string>()
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan, YAKUFUNC f) {
				YKCLASS_GENERAL_INIT;
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
				suppressionList.insert(yk6);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
				suppressionList.insert(yk6); suppressionList.insert(yk7);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, std::string yk8, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
				suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, std::string yk8, std::string yk9, YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
				suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
				suppressionList.insert(yk9);
			}
			Yaku(std::string name, YAKU_HAN::HAN cHan, YAKU_HAN::HAN bHan,
				std::string yk1, std::string yk2, std::string yk3, std::string yk4, std::string yk5,
				std::string yk6, std::string yk7, std::string yk8, std::string yk9, std::string yk10,
				YAKUFUNC f)
			{
				YKCLASS_GENERAL_INIT; suppressionList.insert(yk1); suppressionList.insert(yk2);
				suppressionList.insert(yk3); suppressionList.insert(yk4); suppressionList.insert(yk5);
				suppressionList.insert(yk6); suppressionList.insert(yk7); suppressionList.insert(yk8);
				suppressionList.insert(yk9); suppressionList.insert(yk10);
			}
#undef YKCLASS_GENERAL_INIT
		};

	}
}
#endif
