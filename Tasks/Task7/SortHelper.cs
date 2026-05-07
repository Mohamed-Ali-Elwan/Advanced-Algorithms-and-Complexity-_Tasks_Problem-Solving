using System;
using System.Collections.Generic;

namespace Task7_2200487
{

    internal static class SortHelper
    {
        public static void MergeSort<T>(T[] array, Comparison<T> comparison)
        {
            if (array.Length <= 1) return;
            var temp = new T[array.Length];
            MergeSortArray(array, temp, 0, array.Length - 1, comparison);
        }

        private static void MergeSortArray<T>(T[] arr, T[] temp, int left, int right, Comparison<T> cmp)
        {
            if (left >= right) return;

            int mid = left + (right - left) / 2;
            MergeSortArray(arr, temp, left, mid, cmp);
            MergeSortArray(arr, temp, mid + 1, right, cmp);
            MergeArray(arr, temp, left, mid, right, cmp);
        }

        private static void MergeArray<T>(T[] arr, T[] temp, int left, int mid, int right, Comparison<T> cmp)
        {

            for (int k = left; k <= right; k++)
                temp[k] = arr[k];

            int i = left, j = mid + 1, idx = left;

            while (i <= mid && j <= right)
            {
                if (cmp(temp[i], temp[j]) <= 0)
                    arr[idx++] = temp[i++];
                else
                    arr[idx++] = temp[j++];
            }

            while (i <= mid)
                arr[idx++] = temp[i++];

            while (j <= right)
                arr[idx++] = temp[j++];
        }

        public static void MergeSort<T>(List<T> list, Comparison<T> comparison)
        {
            if (list.Count <= 1) return;
            var temp = new T[list.Count];
            MergeSortList(list, temp, 0, list.Count - 1, comparison);
        }

        private static void MergeSortList<T>(List<T> list, T[] temp, int left, int right, Comparison<T> cmp)
        {
            if (left >= right) return;

            int mid = left + (right - left) / 2;
            MergeSortList(list, temp, left, mid, cmp);
            MergeSortList(list, temp, mid + 1, right, cmp);
            MergeList(list, temp, left, mid, right, cmp);
        }

        private static void MergeList<T>(List<T> list, T[] temp, int left, int mid, int right, Comparison<T> cmp)
        {
            for (int k = left; k <= right; k++)
                temp[k] = list[k];

            int i = left, j = mid + 1, idx = left;

            while (i <= mid && j <= right)
            {
                if (cmp(temp[i], temp[j]) <= 0)
                    list[idx++] = temp[i++];
                else
                    list[idx++] = temp[j++];
            }

            while (i <= mid)
                list[idx++] = temp[i++];

            while (j <= right)
                list[idx++] = temp[j++];
        }
    }
}
