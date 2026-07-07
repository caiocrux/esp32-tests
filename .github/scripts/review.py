import os

from github import Github
from openai import OpenAI

client = OpenAI(api_key=os.environ["OPENAI_API_KEY"])

github = Github(os.environ["GITHUB_TOKEN"])

repo = github.get_repo(os.environ["GITHUB_REPOSITORY"])

pr = repo.get_pull(int(os.environ["PR_NUMBER"]))

files = []

for f in pr.get_files():
    if f.patch:
        files.append(
            f"""
File: {f.filename}

{f.patch}
"""
        )

diff = "\n\n".join(files)

prompt = f"""
You are a senior embedded software engineer.

Review the following GitHub Pull Request.

Focus on:

- correctness
- bugs
- edge cases
- memory leaks
- C/C++ best practices
- ESP-IDF best practices
- FreeRTOS issues
- concurrency
- readability

Only report actionable issues.

PR Diff:

{diff}
"""

response = client.responses.create(
    model="gpt-5",
    input=prompt,
)

review = response.output_text

pr.create_issue_comment(
    f"""## 🤖 AI Code Review

{review}
"""
)
