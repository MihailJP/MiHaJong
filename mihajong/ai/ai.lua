-- 仮置きスクリプト

function determine_discard (gametbl)
	-- 本来はここに捨牌を決定する処理を書く
	return mihajong.DiscardType.Normal, 13 -- 普通にツモ切りする。Luaらしくオリジンを1にしたほうがいいのかなぁ？
end
