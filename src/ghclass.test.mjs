import assert from 'node:assert';
import { test } from 'node:test';
import fs from 'node:fs/promises';

import * as ghclass from './ghclass.mjs';

const csvIndividual = `"identifier","github_username","github_id","name"
"shanepanter@u.boisestate.edu","BSU-ShanePanter","123456","shane"
"student1@u.boisestate.edu","student1","111111","student"`;

test('individual student roster from csv', async (t) => {
  t.mock.method(fs, 'readFile', async () => csvIndividual);
  const result = await ghclass.loadRosterCSV('test');
  assert.deepStrictEqual(
    result,
    [
      {
        email: 'shanepanter@u.boisestate.edu',
        githubUserName: 'BSU-ShanePanter',
        githubID: '123456',
        name: 'shane',
        team: null,
      },
      {
        email: 'student1@u.boisestate.edu',
        githubUserName: 'student1',
        githubID: '111111',
        name: 'student',
        team: null,
      },
    ],
  );
});

test('individual construct repo url from student', () => {
  const org = 'shanep-classroom';
  const project = 'project1';
  const student = {
    email: 'student1@u.boisestate.edu',
    githubUserName: 'student1',
    githubID: '111111',
    name: 'student',
    team: null,
  };
  const actual = ghclass.gitHubRepoUrlSSH(student, org, project);
  const expected = 'git@github.com:shanep-classroom/project1-student1 student1@u.boisestate.edu';
  assert.strictEqual(actual, expected);
});

const csvGroup = `"identifier","github_username","github_id","name","group_name"
"shanepanter@u.boisestate.edu","BSU-ShanePanter","123456","shane","team-001"
"student1@u.boisestate.edu","student1","111111","student","team 002"`;

test('group student roster from csv', async (t) => {
  t.mock.method(fs, 'readFile', async () => csvGroup);
  const result = await ghclass.loadRosterCSV('test');
  assert.deepStrictEqual(
    result,
    [
      {
        email: 'shanepanter@u.boisestate.edu',
        githubUserName: 'BSU-ShanePanter',
        githubID: '123456',
        name: 'shane',
        team: 'team-001',
      },
      {
        email: 'student1@u.boisestate.edu',
        githubUserName: 'student1',
        githubID: '111111',
        name: 'student',
        team: 'team-002',
      },
    ],
  );
});

test('group construct repo url from student', () => {
  const org = 'shanep-classroom';
  const project = 'project1';
  const student = {
    email: 'student1@u.boisestate.edu',
    githubUserName: 'student1',
    githubID: '111111',
    name: 'student',
    team: 'team-002',
  };
  const actual = ghclass.gitHubRepoUrlSSH(student, org, project);
  const expected = 'git@github.com:shanep-classroom/project1-team-002 team-002';
  assert.strictEqual(actual, expected);
});
