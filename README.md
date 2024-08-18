# Plane Shooter Game

## Giới thiệu về bản thân
- Họ tên: Nguyễn Văn Hải
* Mã số sinh viên: 21021487

## Giới thiệu cơ bản về game

Plane Shooter là một trò chơi điện tử được phát triển bằng ngôn ngữ lập trình C++ và thư viện SDL, nơi người chơi điều khiển một chiếc máy bay chiến đấu và tiêu diệt các kẻ địch để ghi điểm. Người chơi sẽ phải đối mặt với những làn sóng kẻ địch xuất hiện ngày càng dày đặc, đòi hỏi sự nhanh nhạy và kỹ năng điều khiển chính xác. Mục tiêu của trò chơi là sống sót càng lâu càng tốt, tiêu diệt nhiều kẻ địch để đạt được số điểm cao nhất.

Trong suốt quá trình chơi, người chơi có thể thu thập các vật phẩm đặc biệt xuất hiện ngẫu nhiên trên màn hình, giúp cải thiện sức mạnh tấn công hoặc phòng thủ của máy bay, chẳng hạn như bắn đạn nhanh hơn, bắn nhiều đạn cùng lúc, hoặc tạo ra một lớp lá chắn bảo vệ tạm thời. Điều này tạo ra sự hấp dẫn và phụ thuộc vào sự may mắn của người chơi.

Với đồ họa 2D đơn giản nhưng đẹp mắt, cùng với âm thanh sống động, Plane Shooter hứa hẹn mang đến những giờ phút giải trí thú vị, đầy kịch tính cho người chơi.

Video giới thiệu về game: [(https://drive.google.com/drive/folders/1DJ1fzuAlyfTpZR5PW7bsUcQZpnQPSTu_)](https://drive.google.com/drive/folders/1DJ1fzuAlyfTpZR5PW7bsUcQZpnQPSTu_)

## Cấu trúc game
- Collision.cpp: Kiểm tra va chạm giữa các thực thể trong game, chẳng hạn như đạn hoặc đồ của địch và máy bay của người chơi, ...
* Dan.cpp: Vẽ đạn của người chơi và kẻ địch, kiểm soát các thông tin của đạn như tốc độ, ...
+ Enemy.cpp: Vẽ kẻ địch và kiểm soát các thông tin của kẻ địch như tần suất xuất hiện mỗi loại kẻ địch, tốc độ di chuyển, ...


- Graphics.cpp: Vẽ màn hình menu, màn hình kết thúc, hearts, kiểm soát việc tải ảnh
* Item.cpp: Vẽ đồ của kẻ địch khi bị tiêu diệt (loại fast và strong), kiểm soát các thông tin của đồ như tốc độ rơi, ...

+ Player.cpp: Vẽ máy bay của người chơi

## Cách chơi game
Bạn phải tìm cách kiểm được nhiều điểm nhất có thể bằng cách tiêu diệt liên tục những kẻ địch xuất hiện, né tránh những viên đạn của chúng và tận dụng những lợi phẩm của kẻ địch rơi ra.

- **Di chuyển**: Sử dụng các phím mũi tên để điều khiển hướng di chuyển của máy bay.
- **Bắn**: Nhấn phím Space để bắn đạn (chỉ bắn được 1 viên mỗi giây nếu đang ko sở hữu đồ của kẻ địch loại fast)
- **Mục tiêu**: Tiêu diệt càng nhiều kẻ địch càng tốt mà không để bị bắn trúng.
- **Điểm số**: Mỗi kẻ địch bị tiêu diệt sẽ cộng điểm vào tổng điểm của bạn.

## Các loại kẻ địch

### Loại Basic
  #### Kẻ địch basic, có tốc độ di chuyển trung bình.
  ![453644429_1037715581056706_63211682297500715_n-removebg-preview](https://github.com/user-attachments/assets/8bf5b196-e07a-4666-9020-82f33a365bd3)
### Loại Fast
  #### Kẻ địch fast, có tốc độ di chuyển nhanh, rơi ra item bắn đạn liên tục.
  ![453498087_2181294398872538_7395675315164132269_n-removebg-preview](https://github.com/user-attachments/assets/362eb72a-6ee1-4bb0-a8d1-e5a62147485e)
### Loại Strong
  #### Kẻ địch strong, có tốc độ di chuyển nhanh, rơi ra item khiên bất tử.
  ![453653991_1880510125788386_6864094955909215025_n-removebg-preview](https://github.com/user-attachments/assets/b6f4ac28-bc3a-43ff-a6f6-06c4ebd947b0)

## Các loại item


### Item bắn đạn liên tục
![455112334_1201365014349980_8259644700510078457_n-removebg-preview](https://github.com/user-attachments/assets/af3e5f4b-d318-4d7d-a34b-34753fa8a486)

### Item khiên bất tử
![454156063_1396139281079975_7976905351537259216_n-removebg-preview](https://github.com/user-attachments/assets/d91777ea-a9ab-498e-a321-daee84b5c193)

## Cách cài đặt

- Bước 1: Download file về máy
* Bước 2: Giải nén
+ Bước 3: Tìm file sdl_window.exe, mở file ra và tận hưởng trò chơi.

## Kỹ thuật sử dụng

- Sử dụng được một số tính năng trong SDL2.0 : xử lí ảnh, va chạm, chuyển động, hiển thị chữ, âm thanh, thời gian, thao tác chuột, bàn phím,...
* Sử dụng nhiều class, vector, mảng, biến, tách file, nạp chồng toán tử,...
+ Tạo ra các đối tượng máy bay, kẻ địch, các nút bấm,..
- Tự tạo ra menu, nút bằng cách hiển thị khung và chữ chèn lên.
* Dùng các kiến thức toán học, vật lí để tinh toán về tần suất xuất hiện của kẻ địch và đạn của chúng, thời gian tác dụng của item, tốc độ di chuyển,...
+ Tách các file,...

## Một vài hình ảnh về game
- Bắt đầu
![image](https://github.com/user-attachments/assets/2bb9d3b8-a9de-489b-b500-2faec463fadd)
- Vào game
  ![image](https://github.com/user-attachments/assets/22677d51-2937-402b-b06b-991ff1cacdbd)
- Kết thúc
![image](https://github.com/user-attachments/assets/63548a77-75f0-4a89-b89d-d6b3cb712de8)

## Nguồn tham khảo
- Lấy source ảnh, âm thanh và font chữ từ các nguồn: pixabay.com(âm thanh, ảnh), pngtree.com(ảnh), dafont.com(font chữ), ...

## Kết luận
### Điểm tốt
- Dự án này do em hoàn toàn tự làm, tự học hỏi và tìm tòi không sao chép ở bất cứ đâu.
* Cải thiện tư duy lên kế hoạch, phát triển sản phẩm, phân chia dự án có cấu trúc.
+ Học được cách setup môi trường, sử dụng thư viện.
- Hiểu được các tư duy xử lí logic game, trạng thái của game.
### Điểm hạn chế
- Chia file chưa tối ưu, chưa chia thành từng folder.
* Game chưa được đầu tư quá nhiều vào đồ họa (do máy em yếu ạ :)) ).
### Hướng phát triển game
- Thêm loại kẻ địch, các đồ rơi ra giúp game thêm phần thú vị.
* Thêm máy bay mới với thiết kế và kỹ năng đặc biệt, người chơi sẽ mở khóa khi đạt được thành tích gì đó.
+ Đa dạng hóa chế độ chơi game: chế độ sinh tồn, chế độ vượt qua từng màn chơi, ...
- Chế độ chơi nhiều người cùng nhau chiến đấu.
* Tự thiết kế máy bay của người chơi.
+ Cải thiện đồ họa và tạo thêm hiệu ứng.
