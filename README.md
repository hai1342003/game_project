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
  - Sprite kẻ địch xuất hiện ngẫu nhiên.
  - Đạn (do người chơi và kẻ địch bắn ra).
3. **Màn hình kết thúc game**: Bao gồm tên game, số điểm người chơi đạt được và 2 nút bấm: Play Again và Quit.

## Cách chơi game
- **Di chuyển**: Sử dụng các phím mũi tên để điều khiển hướng di chuyển của máy bay.
- **Bắn**: Nhấn phím Space để bắn đạn.
- **Mục tiêu**: Tiêu diệt càng nhiều kẻ địch càng tốt mà không để bị bắn trúng.
- **Điểm số**: Mỗi kẻ địch bị tiêu diệt sẽ cộng điểm vào tổng điểm của bạn.

