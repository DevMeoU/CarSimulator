export function parseCSV(csvString) {
    // Xoá BOM (nếu có)
    csvString = csvString.replace(/^\uFEFF/, '');

    // Tách chuỗi theo dòng (newline) và lọc bỏ các dòng trống
    const lines = csvString
        .split('\n')
        .filter(line => line.trim() !== '');

    // Tách mỗi dòng theo dấu phẩy, trim khoảng trắng
    const result = lines.map(line => line.split(',').map(item => item.trim()));

    return result;
}
