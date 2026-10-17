import sys

import pdfplumber


def main() -> None:
    sys.stdout.reconfigure(encoding="utf-8")
    pdf_path = sys.argv[1]
    page_index = int(sys.argv[2])
    keys = sys.argv[3:]
    page = pdfplumber.open(pdf_path).pages[page_index]
    print("size", page.width, page.height)
    words = page.extract_words(keep_blank_chars=False, use_text_flow=False)
    points = [word for word in words if any(key in word["text"] for key in keys)]
    for word in points:
        print(
            "%s x0=%.0f x1=%.0f top=%.0f bottom=%.0f"
            % (word["text"], word["x0"], word["x1"], word["top"], word["bottom"])
        )


if __name__ == "__main__":
    main()
