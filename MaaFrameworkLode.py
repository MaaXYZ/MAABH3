#!/usr/bin/env python3
import os
import sys
import urllib.request
import urllib.error
import time
import json
from pathlib import Path
import shutil

basedir = Path(__file__).parent
temp_dir = Path(basedir, "temp")
temp_dir.mkdir(parents=True, exist_ok=True)

def detect_host_triplet():
    import platform
    machine = platform.machine().lower()
    system = platform.system().lower()
    if machine in {"amd64", "x86_64"}:
        machine = "x86_64"
    elif machine in {"arm64", "armv8l", "aarch64"}:
        machine = "aarch64"
    else:
        raise Exception("unsupported architecture: " + machine)
    if system in "linux":
        pass
    elif "windows" in system or 'mingw' in system or 'cygwin' in system:
        system = "win"
    elif system == "darwin":
        system = "macos"
    else:
        raise Exception("unsupported system: " + system)
    return f"{system}-{machine}"

def split_asset_name(name: str):
    remainder, tag_suffix =  name.rsplit("-", 1)
    tag = tag_suffix.rsplit(".", 1)[0]
    if remainder:
        target = remainder.split("-", 1)[1]
        if target:
            return target, tag
    return None, None

def get_browser_download_url(repo_owner: str, repo_name: str, release_tag: str, target_triplet: str):
    url = f"https://api.github.com/repos/{repo_owner}/{repo_name}/releases/tags/{release_tag}"
    req = urllib.request.Request(url)
    token = os.environ.get("GH_TOKEN", os.environ.get("GITHUB_TOKEN", None))
    for _ in range(5):
        if token:
            req.add_header("Authorization", f"Bearer {token}")
        try:
            resp = urllib.request.urlopen(req)
            break
        except urllib.error.HTTPError as e:
            if e.status == 403 and e.headers.get("x-ratelimit-remaining") == "0":
                # rate limit
                t0 = time.time()
                reset_time = t0 + 10
                try:
                    reset_time = int(e.headers.get("x-ratelimit-reset", 0))
                except ValueError:
                    pass
                reset_time = max(reset_time, t0 + 10)
                print(f"rate limit exceeded, retrying after {reset_time - t0:.1f} seconds")
                time.sleep(reset_time - t0)
                continue
            elif e.status == 404:
                print("resource not found")
            raise    
    release_data = json.loads(resp.read())
    assets = release_data.get("assets")
    if assets:
        for asset in assets:
            asset_name = asset.get("name")
            asset_url = asset.get("browser_download_url")
            if asset.get("content_type") == "application/zip":
                target, _ = split_asset_name(asset_name)
                if target == target_triplet:
                    return asset_url
    return None

def format_size(num, suffix="B"):
    for unit in ["", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi"]:
        if abs(num) < 1024.0:
            return f"{num:3.1f}{unit}{suffix}"
        num /= 1024.0
    return f"{num:.1f}Yi{suffix}"

class ProgressHook:
    def __init__(self):
        self.downloaded = 0
        self.last_print = 0
    
    def __call__(self, block, chunk, total):
        self.downloaded += chunk
        t = time.monotonic()
        if t - self.last_print >= 0.5 or self.downloaded == total:
            self.last_print = t
            if total > 0:
                print(f"\r [{self.downloaded / total * 100.0:3.1f}%] {format_size(self.downloaded)} / {format_size(total)}      \r", end='')
        if self.downloaded == total:
            print("")

def download_zip(url:str, repo_name: str, release_tag: str):
    print("downloading from", url)
    filename = f"{repo_name}_{release_tag}.zip"
    local_file = temp_dir / filename
    urllib.request.urlretrieve(url, local_file, reporthook=ProgressHook())
    print("download complete")
    return filename

def extract_zip(filename: str, extract_dir = None):
    print("extracting")
    if extract_dir is None:
        extract_dir = temp_dir
    shutil.unpack_archive(temp_dir / filename, extract_dir)
    print("extract complete")

def rm_temp():
    shutil.rmtree(temp_dir)

def main():
    if len(sys.argv) == 2:
        target_triplet = sys.argv[1]
    else:
        target_triplet = detect_host_triplet()
    print("about to download prebuilt dependency libraries for", target_triplet)
    download_url = get_browser_download_url("MaaAssistantArknights", "MaaFramework", "v0.3.5", target_triplet)
    filename = download_zip(download_url, "MaaFramework", "v0.3.5")
    extract_zip(filename)
    bin_dir = Path(basedir, "3rdparty/bin")
    bin_dir.mkdir(parents=True, exist_ok=True)
    lib_dir = Path(basedir, "3rdparty/lib")
    lib_dir.mkdir(parents=True, exist_ok=True)
    shutil.copytree(temp_dir / "bin", bin_dir, dirs_exist_ok=True)
    shutil.copytree(temp_dir / "lib", lib_dir, dirs_exist_ok=True)
    rm_temp()

if __name__ == "__main__":
    main()