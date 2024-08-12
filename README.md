# Plane Shooter Game

## About me
- Họ tên: Nguyễn Văn Hải
* Mã số sinh viên: 21021487

## Giới thiệu cơ bản về game
Plane Shooter là một trò chơi điều khiển một chiếc máy bay chiến đấu và tiêu diệt kẻ địch để ghi điểm. Trò chơi được phát triển bằng C++ và SDL.

## Cấu trúc game
Game bao gồm màn hình chính, màn hình chơi game và màn hình kết thúc game.
1. **Màn hình chính**: Bao gồm tên game và nút Play, khi bấm vào nút Play sẽ vào màn hình chơi game.
2. **Màn hình chơi game**:
  - Góc bên trái phía trên màn hình có 3 hình trái tim thể hiện số mạng của người chơi, khi bị kẻ địch bắn trúng sẽ mất 1 trái tim, khi hết trái tim sẽ chuyển sang màn hình kết thúc.
  - Góc bên phải phía trên màn hình có số điểm, ban đầu có 0 điểm, khi người chơi bắn chết 1 kẻ địch sẽ được cộng 10 điểm.
  - Sprite máy bay của người chơi.
  - Sprite kẻ địch xuất hiện ngẫu nhiên:
    1. Loại Basic
    2. Loại Fast (khi bị hạ gục sẽ xuất hiện đồ (hình viên đạn) rơi từ trên xuống)
    3. Loại Strong (khi bị hạ gục sẽ xuất hiện đồ (hình khiên) rơi từ trên xuống)
  - Đạn (do người chơi và kẻ địch bắn ra).

  - Người chơi nhặt đồ hình viên đạn sẽ có viên đạn bên trên sprite máy bay của người chơi, trong 3 giây tiếp theo người chơi bấm space liên tục sẽ bắn đạn liên tục.
  - Người chơi nhặt đồ hình khiên sẽ có khiên bên trên sprite máy bay của người chơi, trong 3 giây tiếp theo nếu người chơi bị kẻ địch bắn trúng sẽ ko mất mạng.

3. **Màn hình kết thúc game**: Bao gồm chữ Game Over, số điểm người chơi đạt được và 2 nút bấm: Play Again và Quit.

## Cách chơi game

- **Di chuyển**: Sử dụng các phím mũi tên để điều khiển hướng di chuyển của máy bay.
- **Bắn**: Nhấn phím Space để bắn đạn (chỉ bắn được 1 viên mỗi giây nếu đang ko sở hữu đồ của kẻ địch loại fast)
- **Mục tiêu**: Tiêu diệt càng nhiều kẻ địch càng tốt mà không để bị bắn trúng.
- **Điểm số**: Mỗi kẻ địch bị tiêu diệt sẽ cộng điểm vào tổng điểm của bạn.


## Đồ họa/Icon
- Kẻ địch và máy bay người chơi khi bị tiêu diệt có hiệu ứng phát nổ: explosion.png
- Đạn: bullet.png
- Kẻ địch thường: enemy.png
- Kẻ địch loại fast: fast_enemy.png
- Kẻ địch loại strong: strong_enemy.png
- Mạng của người chơi: heart.png
- Chữ Game Over: gameover.png
- Máy bay người chơi: plane.png
- Đồ rơi ra khi hạ gục kẻ địch loại fast: star.png
- Đồ rơi ra khi hạ gục kẻ địch loại strong: khien.png

## Âm thanh
- Tiếng đạn bắn: bullet.wav
- Tiếng phát nổ: explosion.wav
