static git_diff_options opts;
static git_diff_list *diff;
static git_tree *a, *b;
static diff_expects exp;
	GIT_INIT_STRUCTURE(&opts, GIT_DIFF_OPTIONS_VERSION);

	memset(&exp, 0, sizeof(exp));

	diff = NULL;
	a = NULL;
	b = NULL;
	git_diff_list_free(diff);
	git_tree_free(a);
	git_tree_free(b);


	git_tree *c;
	git_tree *c, *d;
	git_tree *c;
}
void process_tree_to_tree_diffing(
	const char *old_commit,
	const char *new_commit)
{
	g_repo = cl_git_sandbox_init("unsymlinked.git");

	cl_assert((a = resolve_commit_oid_to_tree(g_repo, old_commit)) != NULL);
	cl_assert((b = resolve_commit_oid_to_tree(g_repo, new_commit)) != NULL);

	cl_git_pass(git_diff_tree_to_tree(&diff, g_repo, a, b, &opts));

	cl_git_pass(git_diff_foreach(
		diff, diff_file_cb, NULL, NULL, &exp));
}

void test_diff_tree__symlink_blob_mode_changed_to_regular_file(void)
{
	/*
	* $ git diff  7fccd7..806999
	* diff --git a/include/Nu/Nu.h b/include/Nu/Nu.h
	* deleted file mode 120000
	* index 19bf568..0000000
	* --- a/include/Nu/Nu.h
	* +++ /dev/null
	* @@ -1 +0,0 @@
	* -../../objc/Nu.h
	* \ No newline at end of file
	* diff --git a/include/Nu/Nu.h b/include/Nu/Nu.h
	* new file mode 100644
	* index 0000000..f9e6561
	* --- /dev/null
	* +++ b/include/Nu/Nu.h
	* @@ -0,0 +1 @@
	* +awesome content
	* diff --git a/objc/Nu.h b/objc/Nu.h
	* deleted file mode 100644
	* index f9e6561..0000000
	* --- a/objc/Nu.h
	* +++ /dev/null
	* @@ -1 +0,0 @@
	* -awesome content
	*/

	process_tree_to_tree_diffing("7fccd7", "806999");

	cl_assert_equal_i(3, exp.files);
	cl_assert_equal_i(2, exp.file_status[GIT_DELTA_DELETED]);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_MODIFIED]);
	cl_assert_equal_i(1, exp.file_status[GIT_DELTA_ADDED]);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_TYPECHANGE]);
}

void test_diff_tree__symlink_blob_mode_changed_to_regular_file_as_typechange(void)
{
	/*
	 * $ git diff  7fccd7..a8595c
	 * diff --git a/include/Nu/Nu.h b/include/Nu/Nu.h
	 * deleted file mode 120000
	 * index 19bf568..0000000
	 * --- a/include/Nu/Nu.h
	 * +++ /dev/null
	 * @@ -1 +0,0 @@
	 * -../../objc/Nu.h
	 * \ No newline at end of file
	 * diff --git a/include/Nu/Nu.h b/include/Nu/Nu.h
	 * new file mode 100755
	 * index 0000000..f9e6561
	 * --- /dev/null
	 * +++ b/include/Nu/Nu.h
	 * @@ -0,0 +1 @@
	 * +awesome content
	 * diff --git a/objc/Nu.h b/objc/Nu.h
	 * deleted file mode 100644
	 * index f9e6561..0000000
	 * --- a/objc/Nu.h
	 * +++ /dev/null
	 * @@ -1 +0,0 @@
	 * -awesome content
	*/

	opts.flags = GIT_DIFF_INCLUDE_TYPECHANGE;
	process_tree_to_tree_diffing("7fccd7", "a8595c");

	cl_assert_equal_i(2, exp.files);
	cl_assert_equal_i(1, exp.file_status[GIT_DELTA_DELETED]);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_MODIFIED]);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_ADDED]);
	cl_assert_equal_i(1, exp.file_status[GIT_DELTA_TYPECHANGE]);
}

void test_diff_tree__regular_blob_mode_changed_to_executable_file(void)
{
	/*
	 * $ git diff 806999..a8595c
	 * diff --git a/include/Nu/Nu.h b/include/Nu/Nu.h
	 * old mode 100644
	 * new mode 100755
	 */

	process_tree_to_tree_diffing("806999", "a8595c");

	cl_assert_equal_i(1, exp.files);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_DELETED]);
	cl_assert_equal_i(1, exp.file_status[GIT_DELTA_MODIFIED]);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_ADDED]);
	cl_assert_equal_i(0, exp.file_status[GIT_DELTA_TYPECHANGE]);