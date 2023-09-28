#!/usr/bin/env node
import { Command } from 'commander';
import { createRequire } from 'node:module';
import { cloneAll } from './ghclass.js';

const req = createRequire(import.meta.url);
const pkg = req('../package.json');

const program = new Command();

program
  .name('ghc')
  .description('CLI downloader for github repositories created with classrooms.github.com')
  .version(pkg.version)
  .requiredOption('-r, --roster <name>', 'File name of the class roster (ex. class_roster.csv)')
  .requiredOption('-o, --organization <name>', 'Name of your Github organization')
  .requiredOption('-a, --assignment <name> ', 'Name of the assignment');

if (!process.argv.slice(2).length) {
  program.help();
}
program.parse(process.argv);
const options = program.opts();

await cloneAll(options['organization'], options['assignment'], options['roster']);
console.log('Done!');
