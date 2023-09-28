import assert from 'node:assert';
import { test } from 'node:test';

import * as ghclass from '../src/ghclass.js';

test('individual student roster from csv', async (t) => {
  const result  = await ghclass.loadRosterCSV('test/data/individual.csv');
  assert.deepStrictEqual(
    result,
    [
      {
        email: 'shanepanter@u.boisestate.edu',
        githubUserName: 'BSU-ShanePanter',
        githubID: '123456',
        name: 'shane',
        team: undefined,
      },
      {
        email: 'student1@u.boisestate.edu',
        githubUserName: 'student1',
        githubID: '111111',
        name: 'student',
        team: undefined,
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
    team: undefined,
  };
  const actual = ghclass.gitHubRepoUrlSSH(student, org, project);
  const expected = 'git@github.com:shanep-classroom/project1-student1 student1@u.boisestate.edu';
  assert.strictEqual(actual, expected);
});


test('group student roster from csv', async (t) => {
  const result = await ghclass.loadRosterCSV('test/data/group.csv');
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
