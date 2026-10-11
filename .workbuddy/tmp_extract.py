import fitz, glob, os, sys

base = r"C:\Users\MYlin\Documents\FarmSmart\Datesheet"
pdfs = glob.glob(os.path.join(base, "**", "*.pdf"), recursive=True)
for p in sorted(pdfs):
    name = os.path.relpath(p, base)
    try:
        doc = fitz.open(p)
        print("=" * 80)
        print(f"FILE: {name}  ({doc.page_count} pages)")
        print("=" * 80)
        for i, page in enumerate(doc):
            t = page.get_text().strip()
            if t:
                print(f"\n----- page {i+1} -----")
                print(t)
        doc.close()
    except Exception as e:
        print(f"ERROR {name}: {e}")
