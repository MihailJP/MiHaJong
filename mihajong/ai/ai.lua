-- 仮置きスクリプト

function name ()
	-- 名前表示欄に表示する名前（未実装。将来のための予約）
end

function ontsumo (gametbl)
	-- 本来はここに捨牌を決定する処理を書く
	return mihajong.DiscardType.Normal, 14 -- 普通にツモ切りする
end

function ondiscard (gametbl)
	-- ここに鳴くかどうか決定する処理を書く
	return mihajong.Call.None -- 何もしない
end
