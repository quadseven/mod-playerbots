-- Remove guild-recruitment prompts from the generic suggestion pool.
DELETE FROM `ai_playerbot_texts`
WHERE `name` = 'suggest_something'
  AND `id` IN (741, 743, 801, 816);
