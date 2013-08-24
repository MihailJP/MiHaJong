-- MiHaJong中国麻雀 デフォルトAI

function ontsumo (gametbl) -- ＡＩの打牌
	return mihajong.DiscardType.Normal, 14
end

function ondiscard (gametbl) -- 鳴き
	return mihajong.Call.None -- 仮実装
end

function onkakan (gametbl)
	return mihajong.Call.None -- 仮実装
end

function onankan (gametbl)
	return mihajong.Call.None -- 仮実装
end
