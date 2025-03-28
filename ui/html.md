### 1. Cấu trúc cơ bản của HTML

* **`<!DOCTYPE html>`**

  Khai báo loại tài liệu HTML5. Nó cho trình duyệt biết trang web của bạn sử dụng chuẩn HTML5.
* **`<html>`**

  Thẻ gốc của tài liệu HTML. Tất cả nội dung của trang web (header, body, …) đều được bao bọc bên trong thẻ này.
* **`<head>`**

  Chứa thông tin meta, liên kết đến stylesheet, script, tiêu đề trang và các thông tin không hiển thị trực tiếp trên giao diện.

  * **`<meta charset="UTF-8">`** : Xác định bộ mã ký tự là UTF-8, giúp hiển thị tiếng Việt và các ký tự đặc biệt.
  * **`<title>`** : Xác định tiêu đề trang web, hiển thị trên thanh tiêu đề của trình duyệt.
  * **`<link>`** : Liên kết đến các tài nguyên ngoài như CSS.
  * **`<script>`** : Nhúng hoặc liên kết đến file JavaScript.
* **`<body>`**

  Chứa toàn bộ nội dung chính của trang web mà người dùng nhìn thấy như văn bản, hình ảnh, liên kết, bảng, danh sách, ….

---

### 2. Các thẻ cấu trúc nội dung

* **`<header>`**

  Thường chứa tiêu đề, logo, và các liên kết điều hướng chính của trang. Nó thường xuất hiện ở đầu trang.
* **`<nav>`**

  Dùng để chứa các liên kết điều hướng (menu). Giúp người dùng dễ dàng di chuyển giữa các phần của trang hoặc các trang khác.
* **`<main>`**

  Bao bọc nội dung chính của trang. Theo chuẩn, chỉ có một thẻ `<main>` trên mỗi trang để chứa nội dung độc nhất.
* **`<section>`**

  Được dùng để chia trang thành các phần có chủ đề riêng biệt, thường đi kèm với tiêu đề.
* **`<article>`**

  Đại diện cho nội dung độc lập, có thể được phát hành độc lập (như bài báo, blog post, tin tức).
* **`<aside>`**

  Chứa thông tin phụ, chẳng hạn như sidebar, quảng cáo hoặc các thông tin liên quan nhưng không phải nội dung chính.
* **`<footer>`**

  Thường chứa thông tin bản quyền, liên kết phụ, thông tin liên hệ, và xuất hiện ở cuối trang.
* **`<div>`**

  Thẻ container không mang ý nghĩa ngữ nghĩa (semantic) nhưng rất hữu ích để nhóm các phần tử lại với nhau để tạo cấu trúc hoặc áp dụng CSS.
* **`<span>`**

  Tương tự như `<div>`, nhưng là inline (trong dòng) để nhóm văn bản hoặc các phần tử nhỏ mà không tạo ra một khối riêng biệt.

---

### 3. Các thẻ nội dung văn bản

* **Tiêu đề:**

  `<h1>`, `<h2>`, `<h3>`, `<h4>`, `<h5>`, `<h6>`

  Sử dụng để xác định tiêu đề, với `<h1>` là tiêu đề quan trọng nhất, `<h6>` là ít quan trọng nhất.
* **Đoạn văn:**

  `<p>`

  Dùng để định nghĩa một đoạn văn.
* **Liên kết:**

  `<a href="url">`

  Dùng để tạo liên kết. Thuộc tính `href` chứa URL đích.
* **Hình ảnh:**

  `<img src="image.jpg" alt="Mô tả hình ảnh">`

  Thẻ này hiển thị hình ảnh. Thuộc tính `src` chứa đường dẫn đến hình ảnh, `alt` cung cấp mô tả thay thế.
* **Danh sách:**

  * **Danh sách không thứ tự:** `<ul>` chứa các `<li>`
  * **Danh sách có thứ tự:** `<ol>` chứa các `<li>`

    `<li>` đại diện cho một mục trong danh sách.
* **Bảng:**

  `<table>` chứa các thành phần:

  * `<tr>`: hàng của bảng (table row)
  * `<th>`: ô tiêu đề (table header)
  * `<td>`: ô dữ liệu (table data)

---

### 4. Các thẻ khác

* **`<form>`**

  Dùng để tạo biểu mẫu nhập liệu, có thể chứa các thẻ `<input>`, `<textarea>`, `<button>`, v.v.
* **`<input>`**

  Thẻ nhập liệu cho người dùng, có thể có các thuộc tính như `type`, `name`, `value`, …

  Ví dụ: `<input type="text">`, `<input type="password">`.
* **`<button>`**

  Tạo nút bấm. Có thể sử dụng bên trong `<form>` hoặc độc lập.
* **`<script>`**

  Nhúng hoặc liên kết đến file JavaScript để thêm tính năng động cho trang web.
* **`<style>`**

  Nhúng CSS trực tiếp vào tài liệu HTML.

---

### 5. Các thẻ semantically khác

* **`<article>`** ,  **`<aside>`** ,  **`<section>`** ,  **`<header>`** ,  **`<footer>`** , **`<nav>`** như đã đề cập ở trên giúp cải thiện cấu trúc và khả năng truy cập (accessibility) của trang web.
