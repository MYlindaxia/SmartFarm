import sys

import pdfplumber


def main() -> None:
    sys.stdout.reconfigure(encoding="utf-8")
    pdf_path = sys.argv[1]
    page_index = int(sys.argv[2])
    x0, top, x1, bottom = (float(value) for value in sys.argv[3:7])
    page = pdfplumber.open(pdf_path).pages[page_index]
    for kind, objects in (
        ("line", page.lines),
        ("rect", page.rects),
        ("curve", page.curves),
    ):
        for obj in objects:
            if (
                obj["x0"] >= x0
                and obj["x1"] <= x1
                and obj["top"] >= top
                and obj["bottom"] <= bottom
            ):
                print(
                    "%s x0=%.1f x1=%.1f top=%.1f bottom=%.1f width=%.1f"
                    % (
                        kind,
                        obj["x0"],
                        obj["x1"],
                        obj["top"],
                        obj["bottom"],
                        obj.get("linewidth", 0),
                    )
                )


if __name__ == "__main__":
    main()
