import cv2
import pytesseract

def main() -> int:
    image = cv2.imread("screenshot.png")
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # threshold the image, setting all foreground pixels to 255
    # and all background pixels to 0
    _, thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
    cv2.imwrite("thresh.png", thresh)

    # find contours in the thresholded image
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    # sort contours by area, descending
    contours = sorted(contours, key=cv2.contourArea, reverse=True)[:9*9]

    if len(contours) < 9*9:
        print(f"error: not enough contours ({len(contours)} found)")
        return 1

    cells = [cv2.boundingRect(contour) for contour in contours]
    cells.sort(key=lambda cell: cell[1])

    grid = [[0] * 9 for _ in range(9)]
    border = 4

    for i in range(9):
        # sort the cells for the current row
        row = cells[9*i:9*(i+1)]
        row.sort(key=lambda cell: cell[0])

        for j in range(9):
            # crop the cell from the image
            x, y, w, h = row[j]
            x += border
            y += border
            w -= 2 * border
            h -= 2 * border

            # check if the cell is empty
            ratio = cv2.countNonZero(thresh[y:y+h, x:x+w]) / (w * h)
            if 0.01 < ratio < 0.99:
                print(f"({i}, {j})")
                # apply OCR to the cell
                config = "--psm 10 --oem 3 -c tessedit_char_whitelist=123456789"
                text = pytesseract.image_to_string(image[y:y+h, x:x+w], config=config).strip()
                if text: grid[i][j] = int(text)

    with open("sudoku.txt", "w") as f:
        for row in grid:
            f.write(" ".join(map(str, row)) + "\n")

    return 0

if __name__ == "__main__":
    exit(main())
