from __future__ import annotations

import json
import subprocess
from pathlib import Path
from flask import Flask, render_template, request

BASE_DIR = Path(__file__).resolve().parent
BINARY = BASE_DIR / "memscope_demo"

app = Flask(__name__)


def get_memory_report() -> dict:
    """Run the bundled, fixed C demonstration and parse its JSON report.

    The web app intentionally does not accept or execute uploaded C code.
    """
    if not BINARY.exists():
        raise RuntimeError("The demo binary is missing. Run `make` in the project folder first.")
    result = subprocess.run(
        [str(BINARY)], cwd=BASE_DIR, capture_output=True, text=True, timeout=3, check=True
    )
    report = json.loads(result.stdout)
    entries = report.get("entries", [])
    for item in entries:
        item["size_label"] = f"{item['size']} byte" + ("s" if item["size"] != 1 else "")
    return {"entries": entries}


@app.route("/")
def index():
    try:
        report = get_memory_report()
        error = None
    except (RuntimeError, subprocess.SubprocessError, json.JSONDecodeError) as exc:
        report = {"entries": []}
        error = str(exc)

    entries = report["entries"]
    regions = ["All", "Stack", "Global/Data", "Heap"]
    selected_region = request.args.get("region", "All")
    if selected_region not in regions:
        selected_region = "All"
    visible_entries = entries if selected_region == "All" else [e for e in entries if e["region"] == selected_region]
    counts = {region: sum(1 for e in entries if e["region"] == region) for region in regions[1:]}
    total_bytes = sum(e["size"] for e in entries)
    return render_template(
        "index.html",
        entries=visible_entries,
        all_entries=entries,
        regions=regions,
        selected_region=selected_region,
        counts=counts,
        total_bytes=total_bytes,
        error=error,
    )


if __name__ == "__main__":
    app.run(host="127.0.0.1", port=5000, debug=False)
