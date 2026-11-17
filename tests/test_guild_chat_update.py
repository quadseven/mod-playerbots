# SPDX-License-Identifier: GPL-2.0-only
"""Replay the guild-advertisement updater against representative rows."""

import sqlite3
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
UPDATE = ROOT / "data/sql/playerbots/updates/2026_11_17_00_ai_playerbot_guild_ads.sql"


class GuildChatUpdateTest(unittest.TestCase):
    def test_update_removes_only_the_four_guild_recruitment_rows(self):
        self.assertTrue(UPDATE.is_file(), "guild-advertisement SQL update is missing")

        connection = sqlite3.connect(":memory:")
        connection.executescript(
            "CREATE TABLE ai_playerbot_texts ("
            "id INTEGER PRIMARY KEY, name TEXT NOT NULL, text TEXT NOT NULL);"
        )
        connection.executemany(
            "INSERT INTO ai_playerbot_texts(id, name, text) VALUES (?, ?, ?)",
            [
                (740, "suggest_something", "Looking for a group"),
                (741, "suggest_something", "Looking for a guild"),
                (742, "suggest_something", "Looking for gold"),
                (743, "suggest_something", "Joining a guild"),
                (744, "suggest_something", "Need a friend"),
                (801, "suggest_something", "Guild carry request"),
                (802, "suggest_something", "Questing together"),
                (815, "suggest_something", "Looking for help"),
                (816, "suggest_something", "Leveling guild request"),
                (817, "suggest_something", "Bored"),
                (900, "suggest_faction", "Guild reputation"),
            ],
        )

        connection.executescript(UPDATE.read_text(encoding="utf-8"))
        remaining = connection.execute(
            "SELECT id FROM ai_playerbot_texts ORDER BY id"
        ).fetchall()
        connection.close()

        self.assertEqual(
            [(740,), (742,), (744,), (802,), (815,), (817,), (900,)],
            remaining,
        )


if __name__ == "__main__":
    unittest.main()
