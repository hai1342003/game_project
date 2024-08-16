# Plane Shooter Game

## About me
- Họ tên: Nguyễn Văn Hải
* Mã số sinh viên: 21021487

## Giới thiệu cơ bản về game

Plane Shooter là một trò chơi điện tử được phát triển bằng ngôn ngữ lập trình C++ và thư viện SDL, nơi người chơi điều khiển một chiếc máy bay chiến đấu và tiêu diệt các kẻ địch để ghi điểm. Người chơi sẽ phải đối mặt với những làn sóng kẻ địch xuất hiện ngày càng dày đặc, đòi hỏi sự nhanh nhạy và kỹ năng điều khiển chính xác. Mục tiêu của trò chơi là sống sót càng lâu càng tốt, tiêu diệt nhiều kẻ địch để đạt được số điểm cao nhất.

Trong suốt quá trình chơi, người chơi có thể thu thập các vật phẩm đặc biệt xuất hiện ngẫu nhiên trên màn hình, giúp cải thiện sức mạnh tấn công hoặc phòng thủ của máy bay, chẳng hạn như bắn đạn nhanh hơn, bắn nhiều đạn cùng lúc, hoặc tạo ra một lớp lá chắn bảo vệ tạm thời. Điều này tạo ra sự hấp dẫn và phụ thuộc vào sự may mắn của người chơi.

Với đồ họa 2D đơn giản nhưng đẹp mắt, cùng với âm thanh sống động, Plane Shooter hứa hẹn mang đến những giờ phút giải trí thú vị, đầy kịch tính cho người chơi.

Video giới thiệu về game: ...

## Cấu trúc game
- Collision.cpp: Kiểm tra va chạm giữa các thực thể trong game, chẳng hạn như đạn hoặc đồ của địch và máy bay của người chơi, ...
* Dan.cpp: Vẽ đạn của người chơi và kẻ địch, kiểm soát các thông tin của đạn như tốc độ, ...
+ Enemy.cpp: Vẽ kẻ địch và kiểm soát các thông tin của kẻ địch như tần suất xuất hiện mỗi loại kẻ địch, tốc độ di chuyển, ...


- Graphics.cpp: Vẽ màn hình menu, màn hình kết thúc, hearts, kiểm soát việc tải ảnh
* Item.cpp: Vẽ đồ của kẻ địch khi bị tiêu diệt (loại fast và strong), kiểm soát các thông tin của đồ như tốc độ rơi, ...

+ Player.cpp: Vẽ máy bay của người chơi

## Cách cài đặt
...
## Cách chơi game
Bạn phải tìm cách kiểm được nhiều điểm nhất có thể bằng cách tiêu diệt liên tục những kẻ địch xuất hiện, né tránh những viên đạn của chúng và tận dụng những lợi phẩm của kẻ địch rơi ra.

- **Di chuyển**: Sử dụng các phím mũi tên để điều khiển hướng di chuyển của máy bay.
- **Bắn**: Nhấn phím Space để bắn đạn (chỉ bắn được 1 viên mỗi giây nếu đang ko sở hữu đồ của kẻ địch loại fast)
- **Mục tiêu**: Tiêu diệt càng nhiều kẻ địch càng tốt mà không để bị bắn trúng.
- **Điểm số**: Mỗi kẻ địch bị tiêu diệt sẽ cộng điểm vào tổng điểm của bạn.

## Một vài hình ảnh về game
- Bắt đầu
![image](https://github.com/user-attachments/assets/de88d877-2b4e-4393-b5e1-f14b9be1fc80)

## Nguồn tham khảo
- Lấy source ảnh, âm thanh và font chữ từ các nguồn: pixabay.com, pngtree.com, dafont.com, ...
  
## Âm thanh
- Tiếng đạn bắn: bullet.wav
- Tiếng phát nổ: explosion.wav
