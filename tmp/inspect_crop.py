import sys

import pdfplumber


def main() -> None:
    sys.stdout.reconfigure(encoding="utf-8")
    pdf_path = sys.argv[1]
    page_index = int(sys.argv[2])
    x0, top, x1, bottom = (float(value) for value in sys.argv[3:7])
    page = pdfplumber.open(pdf_path).pages[page_index]
    words = page.extract_words(keep_blank_chars=False, use_text_flow=False)
    selected = [
        word
        for word in words
        if word["x0"] >= x0
        and word["x1"] <= x1
        and word["top"] >= top
        and word["bottom"] <= bottom
    ]
    selected.sort(key=lambda word: (round(word["top"], 1), word["x0"]))
    for word in selected:
        print(
            "%s x0=%.0f x1=%.0f top=%.0f bottom=%.0f"
            % (word["text"], word["x0"], word["x1"], word["top"], word["bottom"])
        )


if __name__ == "__main__":
    main()
