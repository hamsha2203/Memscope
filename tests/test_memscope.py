import json
import subprocess
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
BINARY = ROOT / "memscope_demo"

class MemScopeDemoTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run(["make", "all"], cwd=ROOT, check=True, capture_output=True, text=True)

    def test_demo_emits_valid_json(self):
        result = subprocess.run([str(BINARY)], cwd=ROOT, check=True, capture_output=True, text=True)
        report = json.loads(result.stdout)
        self.assertIn("entries", report)
        self.assertGreaterEqual(len(report["entries"]), 8)

    def test_regions_and_fields_are_present(self):
        result = subprocess.run([str(BINARY)], cwd=ROOT, check=True, capture_output=True, text=True)
        entries = json.loads(result.stdout)["entries"]
        regions = {entry["region"] for entry in entries}
        self.assertEqual(regions, {"Stack", "Global/Data", "Heap"})
        for entry in entries:
            for field in ("name", "type", "value", "address", "region", "size"):
                self.assertIn(field, entry)
            self.assertTrue(entry["address"].startswith("0x") or entry["address"] == "(nil)")
            self.assertGreater(entry["size"], 0)

    def test_heap_allocation_is_reported_separately(self):
        result = subprocess.run([str(BINARY)], cwd=ROOT, check=True, capture_output=True, text=True)
        entries = json.loads(result.stdout)["entries"]
        heap = next(entry for entry in entries if entry["name"] == "heap_values")
        self.assertEqual(heap["region"], "Heap")
        self.assertEqual(heap["size"], 3 * 4 if __import__('struct').calcsize('i') == 4 else 3 * __import__('struct').calcsize('i'))
        self.assertEqual(heap["value"], "[3, 6, 9]")

if __name__ == "__main__":
    unittest.main(verbosity=2)
